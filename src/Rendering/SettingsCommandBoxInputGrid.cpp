#include "Rendering/SettingsCommandBoxInputGrid.hpp"

#include "Commands/Settings/CommandTabFontText.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsCommandBoxInputGrid::SettingsCommandBoxInputGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsCommandBoxInputGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabFontText();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.commandBoxInput->scale));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.commandBoxInput->xOffset));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.commandBoxInput->yOffset));
	}
}
