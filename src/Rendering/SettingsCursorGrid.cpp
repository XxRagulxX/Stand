#include "Rendering/SettingsCursorGrid.hpp"

#include "Commands/Settings/CommandTabCursor.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsCursorGrid::SettingsCursorGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsCursorGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabCursor();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.padding));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.borderWidth));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.borderRounded));
		AddColorCommandRows(items_draft, Theme::kContentWidth, "cursorborder"_J, "Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.borderRainbow));
	}
}
