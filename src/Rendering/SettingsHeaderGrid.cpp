#include "Rendering/SettingsHeaderGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabHeader.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsHeaderGrid::SettingsHeaderGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsHeaderGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabHeader();
		// Header select — CommandHeaderMode is a CommandListSelect (registered
		// under "header"_J), rendered with the cycling left/right buttons.
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, static_cast<int16_t>(kItemH), "header"_J));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.animationSpeed));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.bgBlur));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.openFolder));
	}
}
