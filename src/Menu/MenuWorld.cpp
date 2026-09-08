#include "Menu/MenuWorld.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build now lives in WorldGrid/
	// SpawnPedGrid/WorldIPLsGrid instead; this stays a real Submenu only
	// so a Lua script can still attach into "World" by name).
	World::World() :
#define ICON_FA_GLOBE "\xef\x82\xac"
	    Submenu::Submenu("World", ICON_FA_GLOBE)
	{
	}
};
