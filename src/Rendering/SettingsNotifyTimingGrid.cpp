#include "Rendering/SettingsNotifyTimingGrid.hpp"

#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/SettingsNotifySampleGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsNotifySampleGrid g_SampleContent{};
	}

	SettingsNotifyTimingGrid::SettingsNotifyTimingGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void SettingsNotifyTimingGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "readspeed"_J, "Reading Speed (WPM)", 10));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "readstartdelay"_J, "Reading Start Delay (ms)", 10));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "notifyminduration"_J, "Min Duration (ms)", 100));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "notifymaxduration"_J, "Max Duration (ms)", 100));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Show Sample Notification", &g_SampleContent));
	}
}
