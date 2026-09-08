#include "Menu/MenuPlayers.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build - including the old inline
	// DrawPlayerList()/GetPlayerTags() player list panel - now lives in
	// PlayersGrid/InfoGrid/TrollGrid/ToxicGrid/KickGrid instead; this
	// stays a real Submenu only so a Lua script can still attach into
	// "Players" by name).
	Players::Players() :
#define ICON_FA_USERS "\xef\x83\x80"
	    Submenu::Submenu("Players", ICON_FA_USERS)
	{
	}
}
