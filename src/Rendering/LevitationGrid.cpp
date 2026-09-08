#include "Rendering/LevitationGrid.hpp"

#include "Rendering/GridItemCommandSliderFloat.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	LevitationGrid::LevitationGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void LevitationGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "levitate"_J));

		// One watchCondition() call for all ~15 dependent rows below -
		// registering the same "levitate"_J watch repeatedly per row
		// would just be redundant (same value, checked once already).
		if (!watchCondition("levitate"_J))
			return;

		for (auto hash : {"levitateignorepitch"_J,
		         "keepmomentum"_J,
		         "levitateapplypitch"_J,
		         "levitaterotate"_J,
		         "levitatebuttoninstructions"_J})
		{
			items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, hash));
		}

		for (auto hash : {"levitatespeed"_J,
		         "levitatesprintmultiplier"_J,
		         "levitateaccel"_J,
		         "levitatepassivemin"_J,
		         "levitatepassivemax"_J,
		         "levitateassistup"_J,
		         "levitateassistdown"_J,
		         "levitateassistsnap"_J})
		{
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, hash));
		}

		for (auto hash : {"levitatepassivespeed"_J, "levitateassistdeadzone"_J})
		{
			items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, hash));
		}
	}
}
