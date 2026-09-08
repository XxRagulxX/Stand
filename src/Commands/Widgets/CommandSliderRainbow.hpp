#pragma once
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Util/get_current_time_millis.hpp"

#include <string>
#include <utility>

namespace Stand::StandWidgets
{
	// Ported from real Stand's own CommandSliderRainbow: not a colour
	// itself - a speed control (0 = off, otherwise milliseconds per
	// step) that drives ANOTHER slider's own value up by 1 every that
	// many ms, wrapping back to its minimum at its maximum. Real Stand
	// uses this to auto-cycle a hue/index slider for its own Weapon
	// Tint Rainbow and Hair Rainbow features - this project doesn't
	// have either of those yet, but the widget itself has no dependency
	// on what it's driving.
	//
	// Needs its own continuous per-tick timing (the same "keeps
	// re-applying every tick, not just on change" gap CommandTogglePointer/
	// CommandSliderPointer's sibling files already disclose elsewhere), solved
	// the same safe way as CommandWalkSpeed.cpp/CommandSwimSpeed.cpp's
	// own hidden always-on LoopedCommand - except owned directly by this
	// class (a private nested Ticker member) rather than hand-written at
	// each use site, since unlike Walk Speed/Swim Speed this widget is
	// meant to be reusable.
	class CommandSliderRainbow : public CommandSliderLegacy
	{
	public:
		CommandSliderRainbow(std::string name, std::string label, std::string description, CommandSliderLegacy* target) :
		    CommandSliderLegacy(name, label, description, 0, 1000, 0),
		    m_Target(target),
		    m_Ticker(name + "_tick", label + " Ticker", "Internal - always on, drives " + label + " over time", this)
		{
		}

		// Public only so the private Ticker member below can reach it -
		// nested classes get no special access to their own outer
		// class's private members in C++. Nothing else should call this.
		void Tick()
		{
			if (!m_Target)
				return;

			const auto speedMs = GetState();
			if (speedMs <= 0)
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

			while (m_MsAccumulated >= speedMs)
			{
				m_MsAccumulated -= speedMs;

				const auto max = m_Target->GetMaximum();
				if (max && m_Target->GetState() >= *max)
				{
					const auto min = m_Target->GetMinimum();
					m_Target->SetState(min ? *min : 0);
				}
				else
				{
					m_Target->SetState(m_Target->GetState() + 1);
				}
			}
		}

	private:
		class Ticker : public LoopedCommand
		{
		public:
			Ticker(std::string name, std::string label, std::string description, CommandSliderRainbow* owner) :
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
			CommandSliderRainbow* m_Owner;
		};

		CommandSliderLegacy* m_Target;
		time_t m_LastTick = 0;
		time_t m_MsAccumulated = 0;
		Ticker m_Ticker;
	};
}
