#include "Core/DetourHook.hpp"
#include "Rendering/Renderer.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"
#include "Rendering/InputCapture.hpp"
#include "Rendering/MenuPopup.hpp"

namespace Stand::Hooks
{
	namespace
	{
		// Remembers whether the last VK_ESCAPE keydown got blocked (see
		// Window::WndProc's own comment) so the matching keyup is blocked
		// too, even if whatever closed on the keydown (e.g.
		// MenuCommandConsole::Close()) means the same live-state check
		// would no longer agree by the time the keyup arrives - the game
		// getting a keyup with no keydown it ever saw is a stranger,
		// riskier state to leave it in than just pairing the two.
		bool g_EscapeKeydownWasBlocked = false;
	}

	LRESULT Window::WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		// Reported bug: pressing Escape to close MenuCommandConsole (or
		// MenuCommandBox, or cancel a GridItemTextInput mid-edit -
		// InputCapture::IsTextInputActive() covers all three, see that
		// flag's own class comment - or dismiss MenuPopup) also opened
		// the game's own pause menu on the very same keypress. Renderer::
		// WndProc() below (and everything it calls into - MenuGrid,
		// MenuPopup, MenuCommandBox, MenuCommandConsole, ...) has no way
		// to actually stop that: this function always calls through to
		// the ORIGINAL game window procedure at the bottom regardless of
		// what any of those did with the message first - none of their
		// own "swallows all input" class comments were ever literally
		// true at this level, only true for this project's own further
		// internal handling (e.g. stopping MenuGrid::HandleKey() from
		// also acting on the same key). The pause menu isn't opened
		// through the PAD:: control-action system GUI::RunScriptImpl()
		// already suppresses for exactly this reason (typing into a text
		// field) - it's the game's own WndProc reading VK_ESCAPE
		// directly, which only THIS function, sitting directly in front
		// of that original WndProc, can actually intercept.
		bool blockEscape = false;
		if (wparam == VK_ESCAPE && (umsg == WM_KEYDOWN || umsg == WM_SYSKEYDOWN))
		{
			// Read BEFORE Renderer::WndProc() runs below, not after -
			// that call is what actually closes whichever of these
			// Escape just hit (e.g. MenuCommandConsole::Close() clears
			// InputCapture's own flag as part of closing itself), so
			// checking afterward would always see the just-cleared state
			// and never block anything.
			blockEscape = (Rendering::InputCapture::IsTextInputActive() || Rendering::MenuPopup::IsOpen());
			g_EscapeKeydownWasBlocked = blockEscape;
		}
		else if (wparam == VK_ESCAPE && (umsg == WM_KEYUP || umsg == WM_SYSKEYUP))
		{
			blockEscape = g_EscapeKeydownWasBlocked;
			g_EscapeKeydownWasBlocked = false;
		}

		if (g_Running)
			Renderer::WndProc(hwnd, umsg, wparam, lparam);

		if (blockEscape)
			return 0;

		return Hooking::Get<Window::WndProc>()->Original<WNDPROC>()(hwnd, umsg, wparam, lparam);
	}
}