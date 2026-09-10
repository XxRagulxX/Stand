#include "Rendering/SettingsAddressBarGrid.hpp"

#include "Commands/Settings/CommandTabAddressBar.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/SettingsAddressBarTextGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsAddressBarTextGrid g_TextContent{};
	}

	SettingsAddressBarGrid::SettingsAddressBarGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsAddressBarGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabAddressBar();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.show));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.separator));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.currentListOnly));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.widthScrollbar));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.widthColumns));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.height));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Text", &g_TextContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.cursorPos));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.cursorPosDividers));
	}
}
