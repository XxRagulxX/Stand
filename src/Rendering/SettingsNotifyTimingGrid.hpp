#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Settings > Notifications > Timing - real Stand's own Reading
	// Speed/Reading Start Delay/Min Duration/Max Duration - see
	// Commands/Settings/CommandNotifyTiming.cpp. Real Stand's own "Show
	// Sample Notification" (Small/Medium/Large) isn't ported here - the
	// parent Notifications page's own always-live preview toast already
	// shows what a notification looks like (and "Flash Notification"
	// re-flashes that same preview), so a separate set of sample buttons
	// would just be a second, different notification alongside it.
	class SettingsNotifyTimingGrid : public Grid
	{
	public:
		SettingsNotifyTimingGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
