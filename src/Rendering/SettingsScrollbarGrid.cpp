#include "Rendering/SettingsScrollbarGrid.hpp"

#include "Commands/Settings/CommandTabScrollbar.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsScrollbarGrid::SettingsScrollbarGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsScrollbarGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabScrollbar();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.mode));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.width));
	}
}
