#include "Rendering/SettingsSmallTextGrid.hpp"

#include "Commands/Settings/CommandTabFontText.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsSmallTextGrid::SettingsSmallTextGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsSmallTextGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabFontText();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.smallText->scale));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.smallText->xOffset));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.smallText->yOffset));
	}
}
