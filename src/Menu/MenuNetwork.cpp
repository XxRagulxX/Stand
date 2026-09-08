#include "Menu/MenuNetwork.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build now lives in NetworkGrid/
	// NetworkSpoofingGrid/SavedPlayersGrid/RandomEventsGrid instead -
	// including the dead "act on every player" commands NetworkGrid's
	// own header comment explains were never wired to anything real
	// even here; this stays a real Submenu only so a Lua script can
	// still attach into "Network" by name).
	Network::Network() :
#define ICON_FA_ROUTE "\xef\x9b\xbf"
	    Submenu::Submenu("Network", ICON_FA_ROUTE)
	{
	}
}
