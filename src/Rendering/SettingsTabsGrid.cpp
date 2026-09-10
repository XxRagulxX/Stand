#include "Rendering/SettingsTabsGrid.hpp"

#include "Commands/Settings/CommandTabTabs.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/SettingsTabsTextGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsTabsTextGrid g_TextContent{};
	}

	SettingsTabsGrid::SettingsTabsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsTabsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabTabs();
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "tabs"_J, "Tabs"));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.width));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.height));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "tabsposition"_J, "Position"));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Text", &g_TextContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.alignment));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.leftIcon));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.showName));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.rightIcon));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.iconsTextColour));
	}
}
