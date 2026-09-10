#include "Rendering/SettingsInputKeepCursorGrid.hpp"

#include "Commands/Settings/CommandTabInput.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsInputKeepCursorGrid::SettingsInputKeepCursorGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsInputKeepCursorGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& keepCursor = *Features::GetCommandTabInput().keepCursor;
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, keepCursor.tabs));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, keepCursor.lists));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, keepCursor.reducedHuge));
	}
}
