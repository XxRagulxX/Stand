#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Recovery > Casino: mirrors MenuRecovery.cpp's
	// casinoSlots group - a single unconditional toggle. (The Lucky
	// Wheel/Blackjack/Roulette groups in the original are commented out
	// there too - nothing to port for them.)
	class RecoveryCasinoGrid : public Grid
	{
	public:
		RecoveryCasinoGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
