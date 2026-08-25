#pragma once

#include "Commands/Widgets/CommandListAction.hpp"

#include "Commands/Widgets/CommandListActionItem.hpp"
#include "Weapons/set_aim_mode.hpp"

namespace Stand
{
	class CommandAimMode : public CommandListAction
	{
	public:
		explicit CommandAimMode(CommandList* parent)
			: CommandListAction(parent, LOC("AIM_MODE"), CMDNAMES("targeting"), NOLABEL, {
				{0, LOC("AIM_MODE_0"), CMDNAMES("assisted")},
				{1, LOC("AIM_MODE_1"), CMDNAMES("partialassist")},
				{2, LOC("AIM_MODE_2"), CMDNAMES("assistedfreeaim")},
				{3, LOC("AIM_MODE_3"), CMDNAMES("freeaim")},
			})
		{
		}

		void onItemClick(Click& click, CommandListActionItem* item) final
		{
			CommandListAction::onItemClick(click, item);
			ensureScriptThread(click, [=, this]
			{
				set_aim_mode((int)item->value);
			});
		}
	};
}
