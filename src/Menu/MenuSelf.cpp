#include "Menu/MenuSelf.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build now lives in SelfGrid/
	// WeaponsGrid/OutfitEditorGrid instead; this stays a real Submenu
	// only so a Lua script can still attach into "Self" by name).
	Self::Self() :
#define ICON_FA_USER "\xef\x80\x87"
	    Submenu::Submenu("Self", ICON_FA_USER)
	{
	}
}
