#include "Rendering/FreecamGrid.hpp"

#include "Rendering/GridItemCommandSliderFloat.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	FreecamGrid::FreecamGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void FreecamGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "freecam"_J));

		if (watchCondition("freecam"_J))
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "freecamspeed"_J));
	}
}
