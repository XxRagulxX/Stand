#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Tab target for Vehicle > Spawn > Personal Vehicle: mirrors
	// SpawnVehicle.cpp's RenderSpawnPersonalVehicle() - its search-box +
	// scrollable result list is now a GridItemSelectList over
	// PersonalVehicles::GetPersonalVehicles(), followed by the Settings
	// group's two toggles. Same simplification as VehicleSpawnNewGrid:
	// no Garage dropdown filter alongside the name search.
	//
	// Origin is baked in below the tab strip GridTabbed (VehicleSpawnGrid)
	// draws above it - see VehicleSpawnNewGrid.hpp's identical note.
	class VehicleSpawnPersonalGrid : public Grid
	{
	public:
		VehicleSpawnPersonalGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
