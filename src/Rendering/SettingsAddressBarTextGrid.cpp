#include "Rendering/SettingsAddressBarTextGrid.hpp"

#include "Commands/Settings/CommandTabAddressBar.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsAddressBarTextGrid::SettingsAddressBarTextGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsAddressBarTextGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabAddressBar();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.text->scale));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.text->xOffset));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.text->yOffset));
	}
}
