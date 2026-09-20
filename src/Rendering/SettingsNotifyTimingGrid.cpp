#include "Rendering/SettingsNotifyTimingGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabNotifyTiming.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/SettingsNotifySampleGrid.hpp"
#include "Rendering/Theme.hpp"

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
		auto& tab = Features::GetCommandTabNotifyTiming();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.readSpeed));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.readStartDelay));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.minDuration));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.maxDuration));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Show Sample Notification", &g_SampleContent));
	}
}
