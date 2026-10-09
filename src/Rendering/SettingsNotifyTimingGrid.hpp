#pragma once
#include "Rendering/StandPort/Grid.hpp"
#include "Rendering/StandPort/Position2d.hpp"

namespace Stand::Rendering
{
	// Settings > Notifications > Timing - real Stand's own Reading
	// Speed/Reading Start Delay/Min Duration/Max Duration/Show Sample
	// Notification - see Commands/Settings/CommandNotifyTiming.cpp and
	// CommandNotifySampleNotifications.cpp.
	class SettingsNotifyTimingGrid : public Grid
	{
	public:
		SettingsNotifyTimingGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
