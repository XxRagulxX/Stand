#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	UINT RawInput::GetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader)
	{
		// Reported bug, fixed: this used to zero every mouse click's raw
		// button flags for as long as GUI::IsOpen() was true, regardless
		// of where the click actually landed - shooting, aiming, and any
		// other click-driven action were all silently eaten the entire
		// time the menu was open, not just while actually clicking a menu
		// row. The Grid menu no longer routes mouse clicks to itself at
		// all (see GridRenderer::WndProcImpl's own comment - keyboard-
		// only navigation now, matching the bug report), so there's
		// nothing left needing protection from a click "meant for the
		// menu" firing in-game instead - checked against real Stand's own
		// input-blocking too (origin/stand-reference's src/AntiCheat/
		// NativeTableHooksBuiltin.cpp): it never touches raw mouse input
		// at all, and only ever suppresses game input while its own
		// command box is actively capturing keystrokes
		// (Commandbox::shouldBlockGameInputs()), never merely because its
		// menu is open. This hook is kept only as the real detour target
		// (Hooking::Get<RawInput::GetRawInputData>() still needs
		// somewhere to call through to) - it now does nothing beyond
		// that.
		return Hooking::Get<RawInput::GetRawInputData>()->Original<decltype(&RawInput::GetRawInputData)>()(hRawInput, uiCommand, pData, pcbSize, cbSizeHeader);
	}
}
