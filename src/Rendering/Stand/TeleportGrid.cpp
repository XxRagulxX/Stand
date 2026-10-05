#include "Rendering/Stand/TeleportGrid.hpp"

#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/TeleportSavedGrid.hpp"
#include "Commands/World/CommandTabWorld.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Owned here rather than in MenuGrid.cpp - see Self.cpp's
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
		constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
		auto& tp = *Features::GetCommandTabWorld().places->teleportTo;

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Saved", &g_SavedContent));

		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Misc", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tp.tpWp));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tp.tpObjective));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, tp.autoTpWp));
	}
}
