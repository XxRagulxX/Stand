#include "Menu/MenuVehicle.hpp"

namespace Stand::Submenus
{
	// Empty on purpose - see UIManager.hpp's own class comment for why
	// (every category this used to build now lives in VehicleGrid/
	// VehicleSpawnGrid/VehicleEditorGrid/SavedVehiclesGrid instead; this
	// stays a real Submenu only so a Lua script can still attach into
	// "Vehicle" by name).
	Vehicle::Vehicle() :
#define ICON_FA_CAR "\xef\x86\xb9"
	    Submenu::Submenu("Vehicle", ICON_FA_CAR)
	{
	}
}
