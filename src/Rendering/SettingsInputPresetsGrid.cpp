#include "Rendering/SettingsInputPresetsGrid.hpp"

#include "Commands/Settings/Input/CommandTabInputPresets.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

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
		auto& tab = Features::GetCommandTabInputPresets();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.flexible));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.tkl));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.full));
	}
}
