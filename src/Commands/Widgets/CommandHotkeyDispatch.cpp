#include "Commands/Widgets/CommandHotkeyDispatch.hpp"

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"

#include "Core/Pointers.hpp" // game import in core!
#include "Menu/GUI.hpp"
#include "Scripting/Natives.hpp" // game import in core!
#include "Scripting/ScriptMgr.hpp"

#include <algorithm>
#include <chrono>

namespace Stand
{
	namespace
	{
		using namespace std::chrono_literals;

		// See CommandHotkeyDispatch.hpp's own class comment for why this
		// is a single shared cooldown rather than true per-key press-edge
		// detection - same idiom as Config/HotkeySystem.cpp's own 100ms,
		// just a little longer since a Stand toggle's onEnable()/
		// onDisable() tends to have a more visible in-world effect than
		// the legacy system's own hotkey-bound commands.
		constexpr auto kCooldown = 250ms;

		bool AnyHotkeyPressed(const CommandPhysical* command)
		{
			for (const auto& hotkey : command->hotkeys)
			{
				if (hotkey.isPressedAsync())
					return true;
			}
			return false;
		}
	}

	void CommandHotkeyDispatch::AddCommandImpl(CommandPhysical* command)
	{
		if (command)
			m_Commands.push_back(command);
	}

	void CommandHotkeyDispatch::RemoveCommandImpl(CommandPhysical* command)
	{
		std::erase(m_Commands, command);
	}

	void CommandHotkeyDispatch::RunScript()
	{
		GetInstance().RunScriptImpl();
	}

	void CommandHotkeyDispatch::RunScriptImpl()
	{
		auto lastTrigger = std::chrono::steady_clock::time_point{};

		while (Stand::g_Running)
		{
			// Same gating conditions as Config/HotkeySystem.cpp's own
			// RunScriptImpl() - a hotkey shouldn't fire while typing into
			// an ImGui text field, while the pause menu/SC overlay has
			// input focus, or while some other window is focused.
			if (GetForegroundWindow() == *Stand::Pointers.Hwnd && !HUD::IS_PAUSE_MENU_ACTIVE() && !HUD::IS_SOCIAL_CLUB_ACTIVE()
			    && !Stand::GUI::IsUsingKeyboard() && std::chrono::steady_clock::now() - lastTrigger > kCooldown)
			{
				for (auto* command : m_Commands)
				{
					if (!AnyHotkeyPressed(command))
						continue;

					Click click(CLICK_HOTKEY, TC_SCRIPT_YIELDABLE);
					command->onClick(click);
					click.ensureResponse();
					click.respond();

					lastTrigger = std::chrono::steady_clock::now();
				}
			}

			Stand::Script::current()->yield();
		}
	}
}
