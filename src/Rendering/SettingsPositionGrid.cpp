#include "Rendering/SettingsPositionGrid.hpp"

#include "Commands/Settings/CommandTabPosition.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

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
		auto& tab = Features::GetCommandTabPosition();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.x));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.y));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.mouseMove));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "tabs"_J, "Tabs"));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "tabsposition"_J, "Tabs Position"));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "header"_J, "Header"));
	}
}
