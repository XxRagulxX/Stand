#include "Commands/Game/CommandTabGame.hpp"

namespace Stand
{
	CommandTabGame::CommandTabGame(CommandList* const parent)
		: CommandTab(parent, TAB_GAME, LOC("GME"), { CMDNAME("tgame"), CMDNAME("gameplay") })
	{
	}

	void CommandTabGame::populate()
	{
	}
}
