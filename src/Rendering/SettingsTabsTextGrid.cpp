#include "Rendering/SettingsTabsTextGrid.hpp"

#include "Commands/Settings/CommandTabTabs.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsTabsTextGrid::SettingsTabsTextGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsTabsTextGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabTabs();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.text->scale));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.text->xOffset));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.text->yOffset));
	}
}
