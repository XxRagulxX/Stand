#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Recovery > Daily Activities: mirrors
	// DailyActivities.cpp's BuildDailyActivitiesMenu() in full - its top-
	// level pair of buttons plus all 16 CollapsingHeaderItem sections
	// (Challenges, Hidden Caches, Treasure Chests, Shipwreck, Buried
	// Stashes, Junk Energy Skydives, Time Trials, Exotic Exports, G's
	// Cache, Stash House, Street Dealers, LS Tags, Madrazo Hits, Shoot
	// Animals Photography, Smoke on the Water, Golden Clover). Every row
	// in every section is a plain Bool/List/CommandItem - no
	// ConditionalItem gating anywhere in this category.
	//
	// Kept as one flat scrollable page (a GridItemText header per
	// section, same as every other multi-group Grid in this system -
	// WeaponsGrid, SettingsGameGrid, ...) rather than a GridItemFolder
	// per section: the plan for this category originally expected a
	// couple of sections (Hidden Caches/Treasure Chests/Street Dealers)
	// to need the searchable-list primitive for a long index list, but
	// each of those ListCommands only has 2-10 entries in practice (see
	// CommandDailyActivities.cpp) - a plain GridItemCommandListSelect cycling
	// stepper is plenty, so there was nothing left that actually needed
	// splitting into sub-pages.
	class DailyActivitiesGrid : public Grid
	{
	public:
		DailyActivitiesGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
