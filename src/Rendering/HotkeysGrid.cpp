#include "Rendering/HotkeysGrid.hpp"

#include "Rendering/GridItemText.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
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
	HotkeysGrid::HotkeysGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void HotkeysGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Command hotkeys not available", Theme::kText));
	}
}
