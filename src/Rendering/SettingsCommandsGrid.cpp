#include "Rendering/SettingsCommandsGrid.hpp"

#include "Commands/Settings/CommandTabCommands.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/SettingsCommandsColourCommandsGrid.hpp"
#include "Rendering/SettingsCommandsNumberSlidersGrid.hpp"
#include "Rendering/SettingsCommandsPrefillGrid.hpp"
#include "Rendering/SettingsCommandsTextGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsCommandsTextGrid g_TextContent{};
		SettingsCommandsColourCommandsGrid g_ColourCommandsContent{};
		SettingsCommandsPrefillGrid g_PrefillContent{};
		SettingsCommandsNumberSlidersGrid g_NumberSlidersContent{};
	}

	SettingsCommandsGrid::SettingsCommandsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsCommandsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabCommands();
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Text", &g_TextContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Colour Commands", &g_ColourCommandsContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Prefill Current Value For Command Box", &g_PrefillContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Number Sliders", &g_NumberSlidersContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.selectableDividers));
	}
}
