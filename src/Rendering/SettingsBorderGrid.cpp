#include "Rendering/SettingsBorderGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabBorder.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsBorderGrid::SettingsBorderGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsBorderGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabBorder();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.width));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.rounded));
		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.colour, "Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.rainbow));
	}
}
