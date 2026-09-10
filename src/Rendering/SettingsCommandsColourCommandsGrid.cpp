#include "Rendering/SettingsCommandsColourCommandsGrid.hpp"

#include "Commands/Settings/CommandTabCommands.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsCommandsColourCommandsGrid::SettingsCommandsColourCommandsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsCommandsColourCommandsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabCommands();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.colourCommands->previewTexture));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.colourCommands->previewList));
	}
}
