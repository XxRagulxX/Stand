#include "Rendering/MpSpecialAbilityGrid.hpp"

#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	MpSpecialAbilityGrid::MpSpecialAbilityGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void MpSpecialAbilityGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "mpspecialability"_J, "Enable in MP"));

		if (watchCondition("mpspecialability"_J))
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "selspecialability"_J, "Special Ability"));
	}
}
