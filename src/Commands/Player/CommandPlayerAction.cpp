#include "Commands/Player/CommandPlayerAction.hpp"

#include "Core/AbstractPlayer.hpp"
#include "Commands/Player/CommandListPlayer.hpp"

namespace Stand
{
	CommandPlayerAction::CommandPlayerAction(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, CommandPerm perm, commandflags_t flags)
		: CommandOnPlayer(parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags, perm)
	{
		PC_PTR->registerCommand(this);
	}
}
