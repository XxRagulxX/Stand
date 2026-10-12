#include "Commands/Stand/CommandRootName.hpp"
#include "Commands/Widgets/CommandFlags.hpp"

namespace Stand
{
	CommandRootName::CommandRootName(CommandList* parent)
		: CommandListSelect(parent, LOC("RN"), {}, NOLABEL, {}, 0, CMDFLAGS_LIST_SELECT, CMDFLAG_FEATURELIST_FINISHLIST)
	{
		instance = this;
	}

	void CommandRootName::update()
	{
	}

	void CommandRootName::onChange(Click& click, long long prev_value)
	{
	}

	std::string CommandRootName::getState() const
	{
		return CommandListSelect::getState();
	}

	std::string CommandRootName::getDefaultState() const
	{
		return CommandListSelect::getDefaultState();
	}

	void CommandRootName::setState(Click& click, const std::string& state)
	{
		CommandListSelect::setState(click, state);
	}
}
