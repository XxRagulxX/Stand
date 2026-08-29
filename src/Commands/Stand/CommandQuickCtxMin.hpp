#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Menu/ContextMenu.hpp"

namespace Stand
{
	class CommandQuickCtxMin : public CommandAction
	{
	public:
		explicit CommandQuickCtxMin(CommandList* const parent)
			: CommandAction(parent, LOC("CTX_MIN"))
		{
		}

		void onClick(Click& click) final
		{
			if (click.type == CLICK_MENU)
			{
				openHotkeysList(click.thread_context);
			}
			else
			{
				ContextMenu::doMin(click);
			}
		}
	};
}
