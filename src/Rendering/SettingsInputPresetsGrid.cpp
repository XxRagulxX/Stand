#include "Rendering/SettingsInputPresetsGrid.hpp"

#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsInputPresetsGrid::SettingsInputPresetsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsInputPresetsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "inputpresetflexible"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "inputpresettkl"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "inputpresetfull"_J));
	}
}
