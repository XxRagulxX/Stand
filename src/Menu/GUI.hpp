#pragma once
#include "Rendering/InputCapture.hpp"

#include <windows.h>

namespace Stand
{
	class GUI final
	{
	private:
		GUI();

	public:
		~GUI();

		GUI(const GUI&) = delete;
		GUI(GUI&&) noexcept = delete;
		GUI& operator=(const GUI&) = delete;
		GUI& operator=(GUI&&) noexcept = delete;

		static void Init()
		{
			GetInstance().m_IsOpen = false;
		}

		static bool IsOpen()
		{
			return GetInstance().m_IsOpen;
		}

		static void Toggle()
		{
			GetInstance().m_IsOpen ^= true;
		}

		static bool IsUsingKeyboard()
		{
			return Rendering::InputCapture::IsTextInputActive();
		}

		static void RunScript()
		{
			GetInstance().RunScriptImpl();
		}

		static void SetOnboarding(bool state)
		{
			GetInstance().SetOnboardingImpl(state);
		}

		static bool IsOnboarding()
		{
			return GetInstance().m_Onboarding;
		}

	private:
		void WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		void SetOnboardingImpl(bool state);
		void RunScriptImpl();

		// Shows/hides the real OS cursor to match GUI::IsOpen() - the
		// Grid menu (this project's only native menu now) supports real
		// mouse interaction, same as Stand's own does, so a visible
		// cursor is needed while it's open. See Rendering/RawInput.cpp
		// for the other half of this: raw input still has to keep
		// reporting movement to the game underneath (so the camera isn't
		// dead while the menu is up), just not left-click, or clicking a
		// menu row would also fire whatever's equipped.
		static void ToggleMouse();

		static GUI& GetInstance()
		{
			static GUI i{};

			return i;
		}

	private:
		bool m_IsOpen;
		bool m_Onboarding;
	};
}