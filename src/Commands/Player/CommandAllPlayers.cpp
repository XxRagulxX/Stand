#include "Commands/Player/CommandAllPlayers.hpp"

#include "Localization/lang.hpp"
#include "Network/PlayerProviderAll.hpp"

namespace Stand
{
	CommandAllPlayers::CommandAllPlayers(CommandList* const parent)
		: CommandListPlayer(parent, &_player_provider, LOC("ALLPLY"), CMDNAMES("allplayers"))
	{
		populate(MAX_PLAYERS);
	}

	std::string CommandAllPlayers::getPlayerName() const
	{
		return menu_name.getLocalisedUtf8();
	}

	CommandName CommandAllPlayers::getCommandNamesSuffix() const
	{
		return CMDNAME("all");
	}
}
