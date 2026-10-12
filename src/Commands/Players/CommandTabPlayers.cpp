#include "Commands/Players/CommandTabPlayers.hpp"

namespace Stand
{
	CommandTabPlayers::CommandTabPlayers(CommandList* const parent)
		: CommandTab(parent, TAB_PLAYERS, LOC("PLYLST"), { CMDNAME("tplayers"), CMDNAME("players"), CMDNAME("playerlist") })
	{
	}

	void CommandTabPlayers::populate()
	{
	}
}
