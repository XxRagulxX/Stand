#include "Commands/Stand/CommandSetFont.hpp"

#include "Commands/Widgets/CommandListActionItem.hpp"

namespace Stand
{
	CommandSetFont::CommandSetFont(CommandList* parent)
		: CommandListAction(parent, LOC("STFNT"), CMDNAMES("setfont"), NOLABEL, {}, CMDFLAGS_LIST_ACTION, CMDFLAG_FEATURELIST_SKIP)
	{
	}

	void CommandSetFont::onClick(Click& click)
	{
		CommandListAction::onClick(click);
	}

	void CommandSetFont::onItemClick(Click& click, CommandListActionItem* item)
	{
		CommandListAction::onItemClick(click, item);
	}
}
