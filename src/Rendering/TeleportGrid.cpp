#include "Rendering/TeleportGrid.hpp"

#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/TeleportSavedGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Owned here rather than in MenuGrid.cpp - see SelfGrid.cpp's
		// identical note about WeaponsGrid.
		TeleportSavedGrid g_SavedContent{};
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Each item below specifies its own
	// width (Theme::kContentWidth) rather than the Grid itself, matching
	// Stand's real Grid - see Grid.hpp's class comment.
	TeleportGrid::TeleportGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void TeleportGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Teleport's other category (the hand-rolled "Saved" locations
		// UI) - has its own content Grid, listed at the very top of the
		// whole list rather than at the bottom, so it's always reachable
		// before the plain items below.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Saved", &g_SavedContent));

		// Misc (MenuTeleport.cpp's miscGroup) - tptowaypoint is shown only
		// while autotptowaypoint is *off* (negate); watchCondition() (not
		// GridItemConditional) so it doesn't reserve its own layout slot
		// while hidden.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Misc", Theme::kText));
		if (watchCondition("autotptowaypoint"_J, true))
			items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "tptowaypoint"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "autotptowaypoint"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "tptoobjective"_J));
	}
}
