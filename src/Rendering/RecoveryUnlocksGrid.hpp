#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Recovery > Unlocks: mirrors Unlocker.cpp's
	// BuildUnlockerMenu() in full - Heist & Progress Awards, Clothing/
	// Tattoos & Masks, and Misc Unlocks & Rank. Every item is an
	// unconditional Bool/Int/CommandItem, no ConditionalItem gating
	// anywhere in the original.
	class RecoveryUnlocksGrid : public Grid
	{
	public:
		RecoveryUnlocksGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
