#include "Menu/MenuTeleport.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build - including the old inline
	// RenderCustomTeleport() Saved-locations editor - now lives in
	// TeleportGrid/TeleportSavedGrid instead; this stays a real Submenu
	// only so a Lua script can still attach into "Teleport" by name).
	Teleport::Teleport() :
#define ICON_FA_TELEPORT "\xef\x8f\x85"
	    Submenu::Submenu("Teleport", ICON_FA_TELEPORT)
	{
	}
}
