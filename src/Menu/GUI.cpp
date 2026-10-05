#include "Menu/GUI.hpp"
#include "Scripting/Script.hpp"
#include "Rendering/Renderer.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/NativeHooks.hpp"
#include "Game/ControllerInputs.hpp"
#include "Rendering/MenuPopup.hpp"
#include "Commands/Widgets/ToggleCorrelation.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	namespace
	{
		bool ShouldBlockPauseMenu()
		{
			return Rendering::InputCapture::IsTextInputActive() || Rendering::MenuPopup::IsOpen();
		}

		bool IsFrontendPauseControl(int action)
		{
			auto input = static_cast<ControllerInputs>(action);
			return input == ControllerInputs::INPUT_FRONTEND_PAUSE || input == ControllerInputs::INPUT_FRONTEND_PAUSE_ALTERNATE;
		}

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

		bool IsPhoneControl(int action)
		{
			return static_cast<ControllerInputs>(action) == ControllerInputs::INPUT_PHONE;
		}

		void IsControlPressedHook(rage::scrNativeCallContext* ctx)
		{
			if (GUI::IsOpen() && IsPhoneControl(ctx->GetArg<int>(1)))
				return ctx->SetReturnValue(FALSE);
			return ctx->SetReturnValue(PAD::IS_CONTROL_PRESSED(ctx->GetArg<int>(0), ctx->GetArg<int>(1)));
		}

		void IsControlJustPressedHook(rage::scrNativeCallContext* ctx)
		{
			if (GUI::IsOpen() && IsPhoneControl(ctx->GetArg<int>(1)))
				return ctx->SetReturnValue(FALSE);
			return ctx->SetReturnValue(PAD::IS_CONTROL_JUST_PRESSED(ctx->GetArg<int>(0), ctx->GetArg<int>(1)));
		}
	}

	GUI::GUI() :
	    m_IsOpen(false)
	{
		Renderer::AddWindowProcedureCallback([this](HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
			GUI::WndProc(hwnd, msg, wparam, lparam);
		});

		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_DISABLED_CONTROL_PRESSED, &IsDisabledControlPressedHook);
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_DISABLED_CONTROL_RELEASED, &IsDisabledControlReleasedHook);
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_DISABLED_CONTROL_JUST_PRESSED, &IsDisabledControlJustPressedHook);
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_DISABLED_CONTROL_JUST_RELEASED, &IsDisabledControlJustReleasedHook);
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_CONTROL_PRESSED, &IsControlPressedHook);
		NativeHooks::AddHook(NativeHooks::ALL_SCRIPTS, NativeIndex::IS_CONTROL_JUST_PRESSED, &IsControlJustPressedHook);

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
			Toggle();
			ToggleMouse();
		}
	}

	void GUI::ToggleMouse()
	{
		static bool cursorShown = false;
		const bool want_mouse = GUI::IsOpen();
		if (want_mouse != cursorShown)
		{
			ShowCursor(want_mouse);
			cursorShown = want_mouse;
		}
	}

	void GUI::RunScriptImpl()
	{
		bool prev_open = false;
		bool prev_on_foot = false;
		bool prev_aiming = false;
		bool prev_freeroam = false;
		bool prev_host = false;

		while (g_Running)
		{
			if (GUI::IsOpen())
			{
				PAD::DISABLE_CONTROL_ACTION(0, static_cast<int>(ControllerInputs::INPUT_PHONE), true);
			}

			if (Rendering::InputCapture::IsTextInputActive())
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

			if (!g_gui.commands_with_correlation.empty())
			{
				const bool cur_open = GUI::IsOpen();
				if (cur_open != prev_open)
				{
					g_gui.processToggleCorrelation(TC_SCRIPT_NOYIELD, ToggleCorrelation::MENU_OPEN, cur_open);
					prev_open = cur_open;
				}

				const bool cur_on_foot = ToggleCorrelation::getCurrentValue(ToggleCorrelation::ON_FOOT);
				if (cur_on_foot != prev_on_foot)
				{
					g_gui.processToggleCorrelation(TC_SCRIPT_NOYIELD, ToggleCorrelation::ON_FOOT, cur_on_foot);
					prev_on_foot = cur_on_foot;
				}

				const bool cur_aiming = ToggleCorrelation::getCurrentValue(ToggleCorrelation::AIMING);
				if (cur_aiming != prev_aiming)
				{
					g_gui.processToggleCorrelation(TC_SCRIPT_NOYIELD, ToggleCorrelation::AIMING, cur_aiming);
					prev_aiming = cur_aiming;
				}

				const bool cur_freeroam = ToggleCorrelation::getCurrentValue(ToggleCorrelation::FREEROAM);
				if (cur_freeroam != prev_freeroam)
				{
					g_gui.processToggleCorrelation(TC_SCRIPT_NOYIELD, ToggleCorrelation::FREEROAM, cur_freeroam);
					prev_freeroam = cur_freeroam;
				}

				const bool cur_host = ToggleCorrelation::getCurrentValue(ToggleCorrelation::SESSION_HOST);
				if (cur_host != prev_host)
				{
					g_gui.processToggleCorrelation(TC_SCRIPT_NOYIELD, ToggleCorrelation::SESSION_HOST, cur_host);
					prev_host = cur_host;
				}
			}

			Script::current()->yield();
		}
	}
}
