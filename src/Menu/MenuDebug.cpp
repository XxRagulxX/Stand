#include "Menu/MenuDebug.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build now lives in MiscGrid/
	// GlobalsGrid/LocalsGrid/ScriptsGrid instead; this stays a real
	// Submenu only so a Lua script can still attach into "Debug" by
	// name).
	Debug::Debug() :
#define ICON_FA_ARROW_RIGHT "\xef\x84\xa0"
	    Submenu::Submenu("Debug", ICON_FA_ARROW_RIGHT)
	{
	}
}
