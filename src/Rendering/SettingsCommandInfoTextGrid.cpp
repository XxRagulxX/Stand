#include "Rendering/SettingsCommandInfoTextGrid.hpp"

#include "Commands/Settings/CommandTabCommandInfoText.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsCommandInfoTextGrid::SettingsCommandInfoTextGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsCommandInfoTextGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabCommandInfoText();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.position));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.width));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.padding));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.showHelpText));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.showSyntax));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.showSliderBehaviour));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.showNonUserOnly));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.showOwners));
	}
}
