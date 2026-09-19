#include "Rendering/VehicleGrid.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/VehicleSpawnGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		VehicleSpawnGrid g_SpawnContent{};
	}

	VehicleGrid::VehicleGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void VehicleGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Spawn", &g_SpawnContent));
	}
}
