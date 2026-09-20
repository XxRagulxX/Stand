#include "Rendering/SettingsTexturesGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabTextures.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsTexturesGrid::SettingsTexturesGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsTexturesGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabTextures();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.leftTextures));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.reloadTextures));
	}
}
