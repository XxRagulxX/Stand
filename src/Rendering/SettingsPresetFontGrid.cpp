#include "Rendering/SettingsPresetFontGrid.hpp"

#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

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
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "presetfontyahei"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "presetfontnanumgothic"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "presetfontbevietnamprolight"_J));
	}
}
