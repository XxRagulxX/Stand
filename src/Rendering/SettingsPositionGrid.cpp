#include "Rendering/SettingsPositionGrid.hpp"

#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Origin/spacer match every other content Grid's - see SelfGrid.cpp's
	// identical comment.
	SettingsPositionGrid::SettingsPositionGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsPositionGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "menux"_J, "X", 10));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "menuy"_J, "Y", 10));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "menumousemove"_J, "Move With Mouse"));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "tabs"_J, "Tabs"));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "tabsposition"_J, "Tabs Position"));
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "header"_J, "Header"));
	}
}
