#include "Rendering/SuperRunGrid.hpp"

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

	SuperRunGrid::SuperRunGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SuperRunGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "superrun"_J));

		if (watchCondition("superrun"_J))
		{
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "moverateoverride"_J));
			items_draft.push_back(std::make_unique<GridItemCommandSliderFloat>(Theme::kContentWidth, kItemH, "runsprintswimmultiplier"_J));
		}
	}
}
