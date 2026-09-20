#include "Rendering/SettingsNotificationsGrid.hpp"

#include "Commands/Settings/Appearance/CommandTabNotifications.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/SettingsNotifyPositionGrid.hpp"
#include "Rendering/SettingsNotifySampleGrid.hpp"
#include "Rendering/SettingsNotifyTimingGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		SettingsNotifyPositionGrid g_PositionContent{};
		SettingsNotifySampleGrid g_SampleContent{};
		SettingsNotifyTimingGrid g_TimingContent{};
	}

	SettingsNotificationsGrid::SettingsNotificationsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	bool SettingsNotificationsGrid::IsActive()
	{
		auto* current = MenuNavigation::Current();
		return dynamic_cast<SettingsNotificationsGrid*>(current) != nullptr || dynamic_cast<SettingsNotifyPositionGrid*>(current) != nullptr;
	}

	void SettingsNotificationsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		auto& tab = Features::GetCommandTabNotifications();

		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.type));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Custom Position", &g_PositionContent));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.invertFlow));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.width));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.padding));

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.borderColour, "Border Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.borderRainbow));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyPrimary));

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.flashColour, "Flash Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.flashRainbow));

		AddColorCommandRows(items_draft, Theme::kContentWidth, tab.bgColour, "Background Colour");
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.copyBg));

		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, kItemH, tab.flashNow));

		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Sample Notifications", &g_SampleContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Timing", &g_TimingContent));
	}
}
