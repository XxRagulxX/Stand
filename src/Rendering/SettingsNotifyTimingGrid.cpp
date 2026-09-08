#include "Rendering/SettingsNotifyTimingGrid.hpp"

#include "Rendering/GridItemCommandSlider.hpp"
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

		// Real Stand's own "Show Sample Notification" (Small/Medium/
		// Large) removed - the always-live preview toast (Notifications::
		// SetPreviewActive(), on while this Notifications settings page
		// is open) already shows what a notification looks like, and
		// "Flash Notification" (on the parent page) re-flashes that same
		// preview - a separate set of sample buttons creating their own,
		// different notifications was redundant with (and confusing
		// alongside) that preview, per explicit user feedback.
	}
}
