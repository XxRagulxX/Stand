#include "Rendering/ToxicGrid.hpp"

#include "Rendering/GridItemCommandPlayer.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;
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
	ToxicGrid::ToxicGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void ToxicGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Damage (damage Group) - every item is a PlayerCommandItem.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Damage", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "kill"_J));
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "killexploit"_J));
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "explode"_J));

		// Griefing (griefing Group) - one PlayerCommandItem.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Griefing", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandPlayer>(Theme::kContentWidth, kItemH, "ceokick"_J));
	}
}
