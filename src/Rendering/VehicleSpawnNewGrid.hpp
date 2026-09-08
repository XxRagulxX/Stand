#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Tab target for Vehicle > Spawn > New Vehicle: mirrors
	// SpawnVehicle.cpp's RenderSpawnNewVehicle() - its search-box +
	// scrollable result list (a single ImGuiItem in the original) is now
	// a GridItemSelectList over the same cached vehicleNames/g_VehicleHashes
	// arrays, followed by the Settings group's two toggles.
	//
	// One simplification from the original: no vehicle-class dropdown
	// filter alongside the name search - GridItemSelectList only supports
	// a single search-text filter (see its own class comment), and a
	// second, independent filter axis isn't part of that primitive yet.
	// Name search alone still narrows a ~200-entry list down plenty; add
	// a class filter back if that turns out not to be enough in practice.
	//
	// Origin is baked in below the tab strip GridTabbed (VehicleSpawnGrid)
	// draws above it, not the usual (1438, 587) every root content Grid
	// uses - see GridTabbed's own class comment for why a tab target has
	// to hardcode this itself.
	class VehicleSpawnNewGrid : public Grid
	{
	public:
		VehicleSpawnNewGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
