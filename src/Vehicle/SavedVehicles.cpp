#include "Vehicle/SavedVehicles.hpp"
#include "Commands/CommandToggleLegacy.hpp"

namespace Stand::Submenus
{
	// Used to also be drawn here, by the classic ImGui menu builder this
	// file no longer has (see SavedVehicles.hpp) - left registered since
	// Rendering/SavedVehiclesGrid.cpp still looks it up by name
	// (Commands::GetCommand<CommandToggle>("spawninsidesavedveh"_J)) and
	// draws it as part of the Grid port instead.
	static CommandToggleLegacy spawnInsideSavedVehicle{"spawninsidesavedveh", "Spawn Inside", "Spawn inside the vehicle."};
}
