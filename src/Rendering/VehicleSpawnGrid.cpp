#include "Rendering/VehicleSpawnGrid.hpp"

#include "Rendering/Theme.hpp"
#include "Rendering/VehicleSpawnNewGrid.hpp"
#include "Rendering/VehicleSpawnPersonalGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		// Owned here rather than in MenuGrid.cpp - see SelfGrid.cpp's
		// identical note about WeaponsGrid. Each already hardcodes its
		// own origin below where this Grid's own tab strip draws - see
		// their own class comments.
		VehicleSpawnNewGrid g_NewVehicleContent{};
		VehicleSpawnPersonalGrid g_PersonalVehicleContent{};
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). This Grid's own item list is just
	// the tab strip below - everything else lives in whichever target is
	// active.
	VehicleSpawnGrid::VehicleSpawnGrid() :
	    GridTabbed(Theme::GetContentOrigin(), 3)
	{
	}

	void VehicleSpawnGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		InitTabs(items_draft,
		    static_cast<int16_t>(Theme::kContentItemHeight),
		    {"New Vehicle", "Personal Vehicle"},
		    {&g_NewVehicleContent, &g_PersonalVehicleContent});
	}
}
