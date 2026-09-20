#include "Rendering/SettingsPresetFontGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabPresetFont.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsPresetFontGrid::SettingsPresetFontGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsPresetFontGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabPresetFont();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.yahei));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.nanumGothic));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.beVietnamProLight));
	}
}
