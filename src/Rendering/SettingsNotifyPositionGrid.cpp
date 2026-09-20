#include "Rendering/SettingsNotifyPositionGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabNotifyPosition.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsNotifyPositionGrid::SettingsNotifyPositionGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsNotifyPositionGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabNotifyPosition();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.x));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.y));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.mouseMove));
	}
}
