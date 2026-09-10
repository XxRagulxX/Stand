#include "Rendering/SettingsNotifySampleGrid.hpp"

#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	SettingsNotifySampleGrid::SettingsNotifySampleGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsNotifySampleGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "notifysamplesmall"_J, "Small"));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "notifysamplemedium"_J, "Medium"));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "notifysamplelarge"_J, "Large"));
	}
}
