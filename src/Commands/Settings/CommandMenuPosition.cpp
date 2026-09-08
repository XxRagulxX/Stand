#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Core/Pointers.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <climits>
#include <cstdint>
#include <windows.h>

namespace Stand::Features
{
	namespace
	{
		// Screen-pixel delta -> H-space delta, for Move With Mouse below.
		// Same per-axis scale factor GridRenderer's own (private)
		// SizeH2C already applies to every draw call - confirmed
		// mathematically identical to this simpler min(...) form as long
		// as GridRenderer's own hudCorrection is doing its job (letterboxing
		// a non-16:9 client area down to an exact 16:9 usable region, the
		// same assumption GetResolutionTextScale() there already makes).
		// Kept self-contained here rather than exposed from GridRenderer -
		// this is the one place outside that file needing a raw cursor
		// position at all, now that the menu is otherwise keyboard-only
		// (see GridRenderer::WndProcImpl's own comment on why real mouse
		// support was removed) - Move With Mouse polls the cursor
		// directly instead, deliberately not routed through any window
		// message.
		float GetHSpaceScale()
		{
			const auto resX = static_cast<float>(*Pointers.ScreenResX);
			const auto resY = static_cast<float>(*Pointers.ScreenResY);
			if (resX <= 0.f || resY <= 0.f)
				return 1.f;

			return std::min(resX / Rendering::Theme::kHudWidth, resY / Rendering::Theme::kHudHeight);
		}
	}

	// Ported from real Stand's own CommandMenuPosition/CommandPosition2d
	// (Commands/Stand/CommandMenuPosition.hpp + Commands/Widgets/
	// CommandPosition2d.hpp/.cpp on origin/stand-reference) - X/Y are its
	// own two IntCommands (real Stand's own CommandSlider children,
	// unbounded - SHRT_MIN..SHRT_MAX, same as its own default_pos.x/y
	// range), and Move With Mouse a toggle. Real Stand's own X/Y default
	// to its own MenuGrid::default_origin ({1323, 560}) - this project's
	// own Theme::kDefaultMenuOriginX/Y are the exact same numbers (see
	// that constant's own comment).
	class CommandMenuPositionX : public CommandSliderLegacy
	{
	public:
		CommandMenuPositionX() :
		    CommandSliderLegacy("menux", "X", "The menu's own horizontal position.", SHRT_MIN, SHRT_MAX, Rendering::Theme::kDefaultMenuOriginX)
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		// Same "a loaded config value needs syncing into Theme too, not
		// just a live edit" reasoning as CommandPrimaryColour.cpp's own
		// LoadState() override.
		void LoadState(nlohmann::json& value) override
		{
			CommandSliderLegacy::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			Rendering::Theme::kMenuOriginX = static_cast<int16_t>(GetState());
		}
	};

	class CommandMenuPositionY : public CommandSliderLegacy
	{
	public:
		CommandMenuPositionY() :
		    CommandSliderLegacy("menuy", "Y", "The menu's own vertical position.", SHRT_MIN, SHRT_MAX, Rendering::Theme::kDefaultMenuOriginY)
		{
		}

	protected:
		void OnChange() override
		{
			Sync();
		}

		void LoadState(nlohmann::json& value) override
		{
			CommandSliderLegacy::LoadState(value);
			Sync();
		}

	private:
		void Sync()
		{
			Rendering::Theme::kMenuOriginY = static_cast<int16_t>(GetState());
		}
	};

	static CommandMenuPositionX _MenuPositionX{};
	static CommandMenuPositionY _MenuPositionY{};

	// Ported from real Stand's own CommandPosition2dMouse (Commands/
	// Widgets/CommandPosition2dMouse.hpp/.cpp) - while on, the menu
	// follows the mouse cursor (offset from wherever both were when this
	// was turned on); turning it back off drops the menu at its last
	// position. Real Stand implements this via its own per-frame
	// "present event handler" polling the cursor directly, not a mouse
	// click/drag gesture - same shape here (a hidden always-on
	// LoopedCommand ticker polling GetCursorPos() every game tick while
	// this toggle itself is on), which is why re-adding this doesn't
	// conflict with the menu otherwise being keyboard-only now: nothing
	// here is routed through a window message.
	class CommandMenuMouseMove : public CommandToggleLegacy
	{
	public:
		CommandMenuMouseMove() :
		    CommandToggleLegacy("menumousemove",
		        "Move With Mouse",
		        "While on, moving the mouse drags the whole menu with it - turn this back off to drop it in place.",
		        false),
		    m_Ticker("menumousemove_tick", "Move With Mouse Ticker", "Internal - always on, polls the cursor while Move With Mouse is on", this)
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
				m_StartOriginX = Rendering::Theme::kMenuOriginX;
				m_StartOriginY = Rendering::Theme::kMenuOriginY;
				return;
			}

			const auto scale = GetHSpaceScale();
			const auto deltaX = static_cast<float>(cur.x - m_StartCursor.x) / scale;
			const auto deltaY = static_cast<float>(cur.y - m_StartCursor.y) / scale;

			const auto newX = static_cast<int>(m_StartOriginX + deltaX);
			const auto newY = static_cast<int>(m_StartOriginY + deltaY);

			// Guarded, not unconditional - CommandSlider::SetState() always
			// queues a FiberPool job and marks the command dirty (unlike
			// CommandColourCustom's own equality check), so calling it every
			// tick regardless would spam both the whole time this is on,
			// even while the cursor sits still.
			if (newX != _MenuPositionX.GetState())
				_MenuPositionX.SetState(newX);
			if (newY != _MenuPositionY.GetState())
				_MenuPositionY.SetState(newY);
		}

	private:
		class Ticker : public LoopedCommand
		{
		public:
			Ticker(std::string name, std::string label, std::string description, CommandMenuMouseMove* owner) :
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
			CommandMenuMouseMove* m_Owner;
		};

		bool m_Dragging = false;
		POINT m_StartCursor{};
		int16_t m_StartOriginX = 0;
		int16_t m_StartOriginY = 0;
		Ticker m_Ticker;
	};

	static CommandMenuMouseMove _MenuMouseMove{};
}
