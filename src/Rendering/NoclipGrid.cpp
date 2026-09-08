#include "Rendering/NoclipGrid.hpp"

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

	NoclipGrid::NoclipGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void NoclipGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "noclip"_J));

		if (watchCondition("noclip"_J))
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "noclipspeed"_J));
	}
}
