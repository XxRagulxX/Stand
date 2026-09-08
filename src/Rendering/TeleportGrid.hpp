#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Teleport > Main: mirrors MenuTeleport.cpp's Misc
	// group in full, wired the same way SelfGrid/VehicleGrid wire their
	// own conditional items. The "Saved" category's own hand-rolled
	// ImGui UI is ported too - see TeleportSavedGrid's own class comment.
	class TeleportGrid : public Grid
	{
	public:
		TeleportGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
