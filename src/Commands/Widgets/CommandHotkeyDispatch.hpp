#pragma once
#include <vector>

// Wires CommandPhysical::hotkeys (Menu/Hotkey.hpp - a fully complete,
// real Stand Hotkey class already ported: modifier bits, hold mode,
// repeat, isPressedAsync()/overlapsWith() all real logic, just never
// polled by anything) into an actual input dispatch - the hotkey
// dispatch piece from the Phase 1 plan.
//
// Real Stand's own dispatch (Rendering/Renderer.cpp's keyDown()/keyUp(),
// confirmed against origin/stand-reference) hooks the game window's
// actual WM_KEYDOWN/WM_KEYUP messages and tracks hold-mode release
// state precisely off real key-up events. This project has no
// window-message hook for that, and already has its own working,
// simpler convention for hotkeys instead - Config/HotkeySystem.cpp's
// own RunScriptImpl(): a dedicated script-thread loop polling
// GetAsyncKeyState with a single shared cooldown between triggers
// rather than true press-edge detection. This follows THAT convention
// instead, for consistency with the rest of this codebase, not the
// WndProc one. Disclosed, known gap: unlike real Stand (and unlike
// Hotkey::isHoldMode()'s own intent), holding a key down re-triggers it
// every kCooldown rather than firing once per physical press and once
// more on release - hold-mode hotkeys aren't meaningfully different
// from ordinary ones here yet.
//
// Dispatches uniformly through CommandPhysical::onClick() - matches
// real Stand's own Gui::triggerHotkey() - so this only does something
// for commands that actually override onClick()
// (CommandToggleNoCorrelation flips + calls onEnable()/onDisable()).
// CommandSlider doesn't override onClick() in this project yet (real
// Stand's own CommandSlider::onClick() isn't ported - see
// CommandSlider.hpp's own file comment) - a hotkey bound to a slider
// command is a real, disclosed no-op for now.
namespace Stand
{
	class CommandPhysical;

	class CommandHotkeyDispatch
	{
	public:
		static void AddCommand(CommandPhysical* command)
		{
			GetInstance().AddCommandImpl(command);
		}

		static void RemoveCommand(CommandPhysical* command)
		{
			GetInstance().RemoveCommandImpl(command);
		}

		static void RunScript();

	private:
		std::vector<CommandPhysical*> m_Commands;

		void AddCommandImpl(CommandPhysical* command);
		void RemoveCommandImpl(CommandPhysical* command);
		void RunScriptImpl();

		static CommandHotkeyDispatch& GetInstance()
		{
			static CommandHotkeyDispatch instance{};
			return instance;
		}
	};
}
