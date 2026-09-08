#include "Menu/MenuSettings.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build - including the old inline
	// Hotkeys() capture UI and DrawGUISettingsMenu()'s ImGuiCol_* style
	// editor, both dropped entirely now that HotkeysGrid/CustomizeGrid
	// exist - now lives in HotkeysGrid/SettingsGuiGrid/SettingsGameGrid/
	// CustomizeGrid/LuaScriptsGrid instead; this stays a real Submenu
	// only so a Lua script can still attach into "Settings" by name).
	Settings::Settings() :
#define ICON_FA_GEARS "\xef\x80\x93"
	    Submenu::Submenu("Settings", ICON_FA_GEARS)
	{
	}
}
