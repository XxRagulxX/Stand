#include "Rendering/SettingsInputMouseGrid.hpp"

#include "Commands/Settings/CommandTabInput.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsInputMouseGrid::SettingsInputMouseGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsInputMouseGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabInput();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.mouseSupport->mouse));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.mouseSupport->cursorOnNav));
	}
}
