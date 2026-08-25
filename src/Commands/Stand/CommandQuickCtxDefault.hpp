#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Menu/ContextMenu.hpp"

namespace Stand
{
	class CommandQuickCtxDefault : public CommandAction
	{
	public:
		explicit CommandQuickCtxDefault(CommandList* const parent)
			: CommandAction(parent, LOC("GODFLT"))
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
				ContextMenu::doApplyDefaultState(click);
			}
		}
	};
}
