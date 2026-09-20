#include "Rendering/SettingsPositionGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabPosition.hpp"
#include "Commands/Settings/Appearance/CommandTabTabs.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsPositionGrid::SettingsPositionGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsPositionGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& pos = Features::GetCommandTabPosition();
		auto& tabs = Features::GetCommandTabTabs();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, pos.x));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, pos.y));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, pos.mouseMove));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tabs.visible));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tabs.positionMode));
	}
}
