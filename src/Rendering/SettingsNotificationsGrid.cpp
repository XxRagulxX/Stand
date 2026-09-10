#include "Rendering/SettingsNotificationsGrid.hpp"

#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandColourCustom.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/SettingsNotifyPositionGrid.hpp"
#include "Rendering/SettingsNotifyTimingGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		// Owned here rather than in SettingsGrid.cpp - see SelfGrid.cpp's
		// identical note about WeaponsGrid.
		SettingsNotifyPositionGrid g_PositionContent{};
		SettingsNotifyTimingGrid g_TimingContent{};
	}

	// Origin/spacer match every other content Grid's - see SelfGrid.cpp's
	// identical comment.
	SettingsNotificationsGrid::SettingsNotificationsGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	bool SettingsNotificationsGrid::IsActive()
	{
		// Only one instance of either class ever exists (g_NotificationsContent
		// in SettingsGrid.cpp, g_PositionContent above) - a dynamic_cast
		// identifying "is the type currently on top of MenuNavigation's
		// stack one of these two" is equivalent to (and doesn't need)
		// comparing against that external instance's own pointer
		// directly, which this file has no reach to (SettingsGrid.cpp
		// owns it, same reasoning as every other content Grid here -
		// see SelfGrid.cpp's own note about WeaponsGrid).
		auto* current = MenuNavigation::Current();
		return dynamic_cast<SettingsNotificationsGrid*>(current) != nullptr || dynamic_cast<SettingsNotifyPositionGrid*>(current) != nullptr;
	}

	void SettingsNotificationsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "notifytype"_J, "Type"));

		// Only takes effect with Type set to "Stand, Custom Position" -
		// same as Invert Flow below (see CommandNotifyPosition.cpp/
		// CommandNotifyFlow.cpp's own comments) - not gated behind a
		// GridItemConditional here since both remain meaningful to look
		// at (and safely no-op) regardless of the current Type.
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Custom Position", &g_PositionContent));

		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "notifyinvertflow"_J, "Invert Flow"));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "notifywidth"_J, "Width", 10));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "notifypadding"_J, "Padding", 10));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "notifyborder"_J, "Border Colour");
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "notifyborderrainbow"_J, "Rainbow Mode"));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "notifycopyprimary"_J, "Copy Primary Colour"));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "notifyflash"_J, "Flash Colour");
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "notifyflashrainbow"_J, "Rainbow Mode"));

		AddColorCommandRows(items_draft, Theme::kContentWidth, "notifybg"_J, "Background Colour");
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "notifycopybg"_J, "Copy Background Colour"));

		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "notifyflashnow"_J, "Flash Notification"));

		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Timing", &g_TimingContent));
	}
}
