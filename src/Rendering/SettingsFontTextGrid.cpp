#include "Rendering/SettingsFontTextGrid.hpp"

#include "Commands/Settings/CommandTabFontText.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/SettingsCommandBoxInputGrid.hpp"
#include "Rendering/SettingsPresetFontGrid.hpp"
#include "Rendering/SettingsSmallTextGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsPresetFontGrid g_PresetFontContent{};
		SettingsCommandBoxInputGrid g_CommandBoxInputContent{};
		SettingsSmallTextGrid g_SmallTextContent{};
	}

	SettingsFontTextGrid::SettingsFontTextGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsFontTextGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabFontText();
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "setfont"_J));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Use Preset Font", &g_PresetFontContent));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "reloadfont"_J));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Command Box Input Text", &g_CommandBoxInputContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Small Text", &g_SmallTextContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.showTextBoundingBoxes));
	}
}
