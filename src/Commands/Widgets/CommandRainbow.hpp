#pragma once
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Util/get_current_time_millis.hpp"

#include <DirectXMath.h>
#include <algorithm>
#include <cmath>
#include <string>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandRainbow (Commands/Stand/
	// CommandRainbow.hpp on origin/stand-reference): a speed control
	// (0 = off, otherwise milliseconds per hue-degree step) that cycles
	// a target colour's hue over time while leaving its current
	// saturation/value/alpha untouched - "still allows you to change the
	// saturation, value, and opacity" is real Stand's own description of
	// this exact behaviour. Same "hidden always-on LoopedCommand ticker"
	// shape as CommandSliderRainbow.hpp (this file's own sibling) - see that
	// file's class comment for the full reasoning, identical here, just
	// driving a colour's hue instead of stepping another CommandSlider's
	// value.
	//
	// Unlike real Stand's own CommandRainbow (which, for its Primary
	// Colour specifically, also cross-updates three additional colour
	// pointers - focusTextColour/focusRightTextColour/focusSpriteColour -
	// so the whole focused-row look stays legible against a shifting
	// accent), this only ever touches the one target colour passed in.
	// This renderer doesn't have a focused/unfocused text colour split
	// yet - a real, disclosed, deliberately deferred gap (see this
	// session's own Settings > Appearance scoping) - so there's nothing
	// else here to cross-update.
	class CommandRainbow : public CommandSliderLegacy
	{
	public:
		CommandRainbow(std::string name, std::string label, std::string description, DirectX::XMFLOAT4* target) :
		    CommandSliderLegacy(name, label, description, 0, 1000, 0),
		    m_Target(target),
		    m_Ticker(name + "_tick", label + " Ticker", "Internal - always on, cycles " + label + "'s hue over time", this)
		{
		}

		// Public only so the private Ticker member below can reach it -
		// nested classes get no special access to their own outer
		// class's private members in C++. Nothing else should call this.
		void Tick()
		{
			if (!m_Target)
				return;

			const auto stepMs = GetState();
			if (stepMs <= 0)
			{
				m_LastTick = 0;
				m_MsAccumulated = 0;
				return;
			}

			const auto now = get_current_time_millis();
			if (m_LastTick == 0)
			{
				m_LastTick = now;
				return;
			}

			m_MsAccumulated += now - m_LastTick;
			m_LastTick = now;

			if (m_MsAccumulated < stepMs)
				return;

			const auto steps = m_MsAccumulated / stepMs;
			m_MsAccumulated -= steps * stepMs;

			auto hsv = RgbToHsv(*m_Target);
			hsv.x = std::fmod(hsv.x + static_cast<float>(steps), 360.f);
			*m_Target = HsvToRgb(hsv);
		}

	private:
		class Ticker : public LoopedCommand
		{
		public:
			Ticker(std::string name, std::string label, std::string description, CommandRainbow* owner) :
			    LoopedCommand(std::move(name), std::move(label), std::move(description)),
			    m_Owner(owner)
			{
				m_State = true;
			}

			bool ShouldSaveState() const override
			{
				return false;
			}

		protected:
			void OnTick() override
			{
				m_Owner->Tick();
			}

		private:
			CommandRainbow* m_Owner;
		};

		// x = hue in [0, 360), y = saturation, z = value, both in [0, 1] -
		// w (alpha) passed through unchanged in both directions.
		static DirectX::XMFLOAT4 RgbToHsv(const DirectX::XMFLOAT4& rgb)
		{
			const float maxC = std::max({rgb.x, rgb.y, rgb.z});
			const float minC = std::min({rgb.x, rgb.y, rgb.z});
			const float delta = maxC - minC;

			float hue = 0.f;
			if (delta > 0.f)
			{
				if (maxC == rgb.x)
					hue = 60.f * std::fmod((rgb.y - rgb.z) / delta, 6.f);
				else if (maxC == rgb.y)
					hue = 60.f * (((rgb.z - rgb.x) / delta) + 2.f);
				else
					hue = 60.f * (((rgb.x - rgb.y) / delta) + 4.f);

				if (hue < 0.f)
					hue += 360.f;
			}

			const float sat = (maxC <= 0.f) ? 0.f : (delta / maxC);
			return {hue, sat, maxC, rgb.w};
		}

		static DirectX::XMFLOAT4 HsvToRgb(const DirectX::XMFLOAT4& hsv)
		{
			const float c = hsv.z * hsv.y;
			const float x = c * (1.f - std::fabs(std::fmod(hsv.x / 60.f, 2.f) - 1.f));
			const float m = hsv.z - c;

			float r1 = 0.f, g1 = 0.f, b1 = 0.f;
			if (hsv.x < 60.f)
			{
				r1 = c;
				g1 = x;
			}
			else if (hsv.x < 120.f)
			{
				r1 = x;
				g1 = c;
			}
			else if (hsv.x < 180.f)
			{
				g1 = c;
				b1 = x;
			}
			else if (hsv.x < 240.f)
			{
				g1 = x;
				b1 = c;
			}
			else if (hsv.x < 300.f)
			{
				r1 = x;
				b1 = c;
			}
			else
			{
				r1 = c;
				b1 = x;
			}

			return {r1 + m, g1 + m, b1 + m, hsv.w};
		}

		DirectX::XMFLOAT4* m_Target;
		time_t m_LastTick = 0;
		time_t m_MsAccumulated = 0;
		Ticker m_Ticker;
	};
}
