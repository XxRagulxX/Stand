#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Menu/ContextMenu.hpp"

namespace Stand
{
	class CommandQuickCtxLoad : public CommandAction
	{
	public:
		explicit CommandQuickCtxLoad(CommandList* const parent)
			: CommandAction(parent, LOC("LSTATE"))
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
				ContextMenu::doLoadState(click);
			}
		}
	};
}
