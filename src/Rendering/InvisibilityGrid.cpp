#include "Rendering/InvisibilityGrid.hpp"

#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Same origin every content Grid uses (see MenuGrid.cpp's own
	// comment for why) - this one's just reached via a GridItemFolder
	// row instead of the sidebar.
	InvisibilityGrid::InvisibilityGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void InvisibilityGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "invis"_J));

		// watchCondition() (not GridItemConditional) so this row simply
		// doesn't exist while invis is off, rather than existing but
		// hidden - see its own doc comment in Grid.hpp for why.
		if (watchCondition("invis"_J))
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "localvis"_J));
	}
}
