#include "Rendering/SettingsNotifySampleGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabNotifySample.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsNotifySampleGrid::SettingsNotifySampleGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsNotifySampleGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabNotifySample();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.small_));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.medium));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.large_));
	}
}
