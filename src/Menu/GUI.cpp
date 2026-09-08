#include "Menu/GUI.hpp"
#include "Menu/Menu.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Rendering/Renderer.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/NativeHooks.hpp"
#include "Game/ControllerInputs.hpp"
#include "Rendering/MenuPopup.hpp"

namespace Stand
{
	namespace
	{
		// Whether Escape/frontend-pause should be swallowed rather than
		// actually opening the game's own pause menu - same gate
		// GUI::RunScriptImpl()'s own PAD::DISABLE_ALL_CONTROL_ACTIONS(0)
		// call below uses for InputCapture, plus MenuPopup (also closed
		// with Escape, but never sets InputCapture - it's not a text
		// field).
		bool ShouldBlockPauseMenu()
		{
			return Rendering::InputCapture::IsTextInputActive() || Rendering::MenuPopup::IsOpen();
		}

		bool IsFrontendPauseControl(int action)
		{
			auto input = static_cast<ControllerInputs>(action);
			return input == ControllerInputs::INPUT_FRONTEND_PAUSE || input == ControllerInputs::INPUT_FRONTEND_PAUSE_ALTERNATE;
		}

		// Reported bug: closing MenuCommandConsole/MenuCommandBox/MenuPopup
		// (or cancelling a GridItemTextInput mid-edit) with Escape also
		// opened the game's own pause menu on the same keypress, and did
		// so unreliably from a pure window-message-level fix (Window.cpp's
		// own WndProc hook blocking the raw WM_(SYS)KEYDOWN/UP for
		// VK_ESCAPE) - "closes correctly sometimes, opens the pause menu
		// most of the time" is exactly what a race looks like: the game
		// reads its own frontend-pause control every tick, independent of
		// (and not synchronized with) whenever this project's own WndProc
		// hook happens to run for that same keystroke, so blocking the
		// window message alone only sometimes wins that race.
		//
		// Checked how real Stand actually solves this instead of guessing
		// further - origin/stand-reference's own AntiCheat/
		// NativeTableHooksBuiltin.cpp overrides exactly these four
		// natives (IS_DISABLED_CONTROL_PRESSED/RELEASED/JUST_PRESSED/
		// JUST_RELEASED) to force FALSE while its own command box is
		// capturing keystrokes - the "disabled control" variants exist
		// specifically so frontend/menu-triggering input (like opening
		// the pause menu) keeps responding even while a script has called
		// DISABLE_ALL_CONTROL_ACTIONS for gameplay purposes, which is
		// exactly why this project's own existing DISABLE_ALL_CONTROL_ACTIONS
		// call below (already fixed digits/letters leaking into game
		// controls while typing) never touched this specific case: the
		// frontend-pause read deliberately bypasses it. This project
		// already has its own generic native-hooking system (Scripting/
		// NativeHooks.hpp, used e.g. by CommandMpSpecialAbility.cpp) - no
		// new infrastructure needed, just registering these four the same
		// way real Stand's own NativeTableHookOverride does, scoped to
		// only the frontend-pause control action so nothing else reading
		// a "disabled control" elsewhere is affected.
		void IsDisabledControlPressedHook(rage::scrNativeCallContext* ctx)
		{
			if (ShouldBlockPauseMenu() && IsFrontendPauseControl(ctx->GetArg<int>(1)))
				return ctx->SetReturnValue(FALSE);
			return ctx->SetReturnValue(PAD::IS_DISABLED_CONTROL_PRESSED(ctx->GetArg<int>(0), ctx->GetArg<int>(1)));
		}

		void IsDisabledControlReleasedHook(rage::scrNativeCallContext* ctx)
		{
			if (ShouldBlockPauseMenu() && IsFrontendPauseControl(ctx->GetArg<int>(1)))
				return ctx->SetReturnValue(FALSE);
			return ctx->SetReturnValue(PAD::IS_DISABLED_CONTROL_RELEASED(ctx->GetArg<int>(0), ctx->GetArg<int>(1)));
		}

		void IsDisabledControlJustPressedHook(rage::scrNativeCallContext* ctx)
		{
			if (ShouldBlockPauseMenu() && IsFrontendPauseControl(ctx->GetArg<int>(1)))
				return ctx->SetReturnValue(FALSE);
			return ctx->SetReturnValue(PAD::IS_DISABLED_CONTROL_JUST_PRESSED(ctx->GetArg<int>(0), ctx->GetArg<int>(1)));
		}

		void IsDisabledControlJustReleasedHook(rage::scrNativeCallContext* ctx)
		{
			if (ShouldBlockPauseMenu() && IsFrontendPauseControl(ctx->GetArg<int>(1)))
				return ctx->SetReturnValue(FALSE);
			return ctx->SetReturnValue(PAD::IS_DISABLED_CONTROL_JUST_RELEASED(ctx->GetArg<int>(0), ctx->GetArg<int>(1)));
		}
	}

	GUI::GUI() :
	    m_IsOpen(false)
	{
		Menu::Init();

		Renderer::AddWindowProcedureCallback([this](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
			GUI::WndProc(hwnd, msg, wparam, lparam);
		});

		// ALL_SCRIPTS, not one specific script name - unlike most of this
		// project's own NativeHooks::AddHook() call sites (each gated
		// behind its own feature's own toggle, hooking only the one
		// script it cares about), this needs to affect the frontend-pause
		// read no matter which script happens to be polling it (real
		// Stand's own equivalent is similarly global, an actual native-
		// table override rather than a per-script one), and it's always
		// active rather than opt-in.
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_DISABLED_CONTROL_PRESSED, &IsDisabledControlPressedHook);
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_DISABLED_CONTROL_RELEASED, &IsDisabledControlReleasedHook);
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_DISABLED_CONTROL_JUST_PRESSED, &IsDisabledControlJustPressedHook);
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_DISABLED_CONTROL_JUST_RELEASED, &IsDisabledControlJustReleasedHook);

		Renderer::SetSafeToRender();
	}

	GUI::~GUI()
	{
	}

	void GUI::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (msg == WM_KEYUP
		    && (wparam == VK_INSERT || (wparam == VK_OEM_5 && (GetKeyState(VK_CONTROL) & 0x8000) != 0)))
		{
			// Persist and restore the cursor position between menu instances
			static POINT CursorCoords{};
			if (m_IsOpen)
			{
				GetCursorPos(&CursorCoords);
			}
			else if (CursorCoords.x + CursorCoords.y)
			{
				SetCursorPos(CursorCoords.x, CursorCoords.y);
			}
			if (!GUI::IsOnboarding())
				Toggle();
			ToggleMouse();
		}
	}

	void GUI::ToggleMouse()
	{
		// ShowCursor() maintains an internal display counter rather than
		// a plain on/off flag, so this only calls it on an actual change
		// to avoid that counter drifting if ToggleMouse() is ever called
		// more than once for the same state.
		static bool cursorShown = false;
		const bool want_mouse = GUI::IsOpen();
		if (want_mouse != cursorShown)
		{
			ShowCursor(want_mouse);
			cursorShown = want_mouse;
		}
	}

	void GUI::SetOnboardingImpl(bool state)
	{
		m_Onboarding = state;
	}

	void GUI::RunScriptImpl()
	{
		while (g_Running)
		{
			if (GUI::IsOpen())
			{
				// Real Stand feel: the menu doesn't take over the game's
				// input at all while it's open (driving, aiming, ... all
				// keep working) - it only needs to stop the one control
				// that would otherwise fire alongside its own key reads:
				// Up Arrow (INPUT_PHONE) is what was popping the cell
				// phone open every time Up/Down was used to navigate.
				PAD::DISABLE_CONTROL_ACTION(0, static_cast<int>(ControllerInputs::INPUT_PHONE), true);
			}

			// Reported bug, fixed the same way real Stand does: typing a
			// value into MenuCommandBox (or any GridItemTextInput) was
			// reaching the game's own control reads too - digits opened
			// the weapon wheel and switched weapons, 'P' opened the pause
			// menu - since nothing here ever stopped a keystroke meant
			// for a text field from also being read as a normal game
			// control. Checked how real Stand actually solves this rather
			// than guessing: origin/stand-reference's own src/Rendering/
			// Gui.cpp calls PAD::DISABLE_ALL_CONTROL_ACTIONS(0) every
			// tick for exactly as long as its own
			// Commandbox::shouldBlockGameInputs() (its command box
			// actively capturing keystrokes) is true - this project's own
			// equivalent state is InputCapture::IsTextInputActive() (see
			// GridItemTextInput/MenuCommandBox, both of which already set
			// it while editing). Same call, same gating, every tick that
			// state is true - independent of GUI::IsOpen() above, since
			// Stand's own call isn't nested under a "menu open" check
			// either, just its own narrower text-capture one.
			if (Rendering::InputCapture::IsTextInputActive())
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

			Script::current()->yield();
		}
	}
}
