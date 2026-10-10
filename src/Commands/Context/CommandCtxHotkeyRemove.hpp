#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Rendering/StandPort/CommandAction.hpp"

#include "Commands/Context/CommandCtxHotkey.hpp"
#include "Menu/ContextMenu.hpp"

namespace Stand
{
	class CommandCtxHotkeyRemove : public CommandAction
	{
	public:
		explicit CommandCtxHotkeyRemove(CommandList* parent, CommandPhysical* const target)
			: CommandAction(parent, LOC("HOTKEY_R2"))
		{
		}

		void onClick(Click& click) final
		{
			parent->goBackIfActive(click.thread_context);
			auto* target = ContextMenu::getTargetPhysical();
			target->removeHotkey(*((CommandCtxHotkey*)parent)->hotkey);
			if (target->canCountAsCommandWithHotkeys() && target->hotkeys.empty())
			{
				target->removeFromCommandsWithHotkeys();
			}
			((CommandCtxHotkey*)parent)->save(target);
		}
	};
}
