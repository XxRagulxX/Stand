#pragma once
#include "Rendering/StandPort/Grid.hpp"
#include "Rendering/StandPort/Position2d.hpp"

namespace Stand::Rendering
{
	// Settings > Notifications > Custom Position - real Stand's own
	// CommandNotifyPosition (X/Y + Move With Mouse) - see
	// Commands/Settings/CommandNotifyPosition.cpp. Only takes effect
	// while Notify Type is set to "Stand, Custom Position" - same
	// shape as SettingsPositionGrid (the menu's own equivalent), just
	// targeting NotifySettings::kCustomPositionX/Y instead of the
	// menu's own Theme::kMenuOriginX/Y.
	class SettingsNotifyPositionGrid : public Grid
	{
	public:
		SettingsNotifyPositionGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
