#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Settings > Game: three always-visible folder rows
	// (Player ESP, Ped ESP, Object ESP) each drilling into a sub-grid
	// that shows all options unconditionally.
	class SettingsGameGrid : public Grid
	{
	public:
		SettingsGameGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
