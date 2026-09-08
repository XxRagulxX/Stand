#pragma once
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Core/Pointers.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <string>
#include <utility>
#include <windows.h>

namespace Stand::StandWidgets
{
	namespace Detail
	{
		// Screen-pixel delta -> H-space delta, for CommandPosition2dMouse
		// below. Same per-axis scale factor GridRenderer's own (private)
		// SizeH2C already applies to every draw call - confirmed
		// mathematically identical to this simpler min(...) form as long
		// as GridRenderer's own hudCorrection is doing its job (letterboxing
		// a non-16:9 client area down to an exact 16:9 usable region, the
		// same assumption GetResolutionTextScale() there already makes).
		// Kept self-contained here rather than exposed from GridRenderer -
		// this (and the menu's own Move With Mouse, this file's first use
		// site) are the only places outside that file needing a raw
		// cursor position at all, now that the menu is otherwise
		// keyboard-only (see GridRenderer::WndProcImpl's own comment on
		// why real mouse support was removed) - both poll the cursor
		// directly instead, deliberately not routed through any window
		// message.
		inline float GetHSpaceScale()
		{
			const auto resX = static_cast<float>(*Pointers.ScreenResX);
			const auto resY = static_cast<float>(*Pointers.ScreenResY);
			if (resX <= 0.f || resY <= 0.f)
				return 1.f;

			return std::min(resX / Rendering::Theme::kHudWidth, resY / Rendering::Theme::kHudHeight);
		}
	}

	// One axis (X or Y) of a real Stand-style CommandPosition2d - an
	// unbounded CommandSlider (SHRT_MIN..SHRT_MAX, matching real Stand's
	// own Position2d int16_t fields) that writes straight into whatever
	// int16_t the caller points it at, syncing both on a live edit and
	// on a loaded config value (see CommandPositionAxis::LoadState()'s
	// own comment for why the latter needs its own explicit sync).
	class CommandPositionAxis : public CommandSliderLegacy
	{
	public:
		CommandPositionAxis(std::string name, std::string label, std::string description, int16_t* target, int16_t defaultValue) :
		    CommandSliderLegacy(std::move(name), std::move(label), std::move(description), SHRT_MIN, SHRT_MAX, defaultValue),
		    m_Target(target)
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		// Same "a loaded config value needs syncing into the real
		// target too, not just a live edit" reasoning as
		// CommandPrimaryColour.cpp's own LoadState() override -
		// OnChange() above only ever fires from SetState() (see
		// CommandSlider::SetState()'s own FiberPool::queueJob call), which
		// a config load never goes through (LoadState() writes m_State
		// directly).
		void LoadState(nlohmann::json& value) override
		{
			CommandSliderLegacy::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			if (m_Target)
				*m_Target = static_cast<int16_t>(GetState());
		}

		int16_t* m_Target;
	};

	// Real Stand's own CommandPosition2dMouse (Commands/Widgets/
	// CommandPosition2dMouse.hpp/.cpp on origin/stand-reference) - while
	// on, whatever this is attached to follows the mouse cursor (offset
	// from wherever both were when this was turned on); turning it back
	// off drops it at its last position. Real Stand implements this via
	// its own per-frame "present event handler" polling the cursor
	// directly, not a mouse click/drag gesture - same shape here (a
	// hidden always-on LoopedCommand ticker polling GetCursorPos() every
	// game tick while this toggle itself is on).
	class CommandPosition2dMouse : public CommandToggleLegacy
	{
	public:
		CommandPosition2dMouse(std::string name, std::string label, std::string description, CommandPositionAxis* x, CommandPositionAxis* y) :
		    CommandToggleLegacy(name, label, description, false),
		    m_X(x),
		    m_Y(y),
		    m_Ticker(name + "_tick", label + " Ticker", "Internal - always on, polls the cursor while " + label + " is on", this)
		{
		}

		// Public only so the private Ticker member below can reach it -
		// nested classes get no special access to their own outer
		// class's private members in C++. Nothing else should call this.
		//
		// No OnEnable() override capturing the start position - real
		// Stand's own onEnable()/onDisable() there manage a "mouse
		// navigation mode" flag this project has no equivalent of (mouse
		// doesn't drive menu navigation at all any more - see
		// GridRenderer::WndProcImpl's own comment). CommandToggle::
		// SetState()'s own OnEnable() call only ever runs asynchronously
		// (FiberPool::queueJob), so a tick could run before it does,
		// reading a stale (possibly zeroed, possibly a previous drag's)
		// start position - captured here instead, synchronously, the
		// first tick m_Dragging notices GetState() went true, same
		// "detect the transition on the next tick rather than trust an
		// async callback's timing" idiom CommandRainbow/CommandSliderRainbow's
		// own m_LastTick == 0 first-tick check already uses.
		void Tick()
		{
			if (!GetState())
			{
				m_Dragging = false;
				return;
			}

			POINT cur{};
			GetCursorPos(&cur);

			if (!m_Dragging)
			{
				m_Dragging = true;
				m_StartCursor = cur;
				m_StartX = m_X ? static_cast<int16_t>(m_X->GetState()) : 0;
				m_StartY = m_Y ? static_cast<int16_t>(m_Y->GetState()) : 0;
				return;
			}

			const auto scale = Detail::GetHSpaceScale();
			const auto deltaX = static_cast<float>(cur.x - m_StartCursor.x) / scale;
			const auto deltaY = static_cast<float>(cur.y - m_StartCursor.y) / scale;

			const auto newX = static_cast<int>(m_StartX + deltaX);
			const auto newY = static_cast<int>(m_StartY + deltaY);

			// Guarded, not unconditional - CommandSlider::SetState() always
			// queues a FiberPool job and marks the command dirty (unlike
			// CommandColourCustom's own equality check), so calling it every
			// tick regardless would spam both the whole time this is on,
			// even while the cursor sits still.
			if (m_X && newX != m_X->GetState())
				m_X->SetState(newX);
			if (m_Y && newY != m_Y->GetState())
				m_Y->SetState(newY);
		}

	private:
		class Ticker : public LoopedCommand
		{
		public:
			Ticker(std::string name, std::string label, std::string description, CommandPosition2dMouse* owner) :
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
			CommandPosition2dMouse* m_Owner;
		};

		CommandPositionAxis* m_X;
		CommandPositionAxis* m_Y;
		bool m_Dragging = false;
		POINT m_StartCursor{};
		int16_t m_StartX = 0;
		int16_t m_StartY = 0;
		Ticker m_Ticker;
	};
}
