#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Recovery > Main: mirrors MenuRecovery.cpp's
	// generalGroup in full, wired the same way SelfGrid/VehicleGrid wire
	// their own conditional items.
	//
	// Every category folder now points at a real content Grid
	// (RecoveryBusinessesGrid/RecoveryHeistsGrid/DailyActivitiesGrid/
	// StatEditorGrid/TransactionsGrid/RecoveryCasinoGrid/
	// RecoveryUnlocksGrid, owned here since nothing else reaches them) -
	// none of Recovery's own categories still fall back to the shared
	// placeholder.
	class RecoveryGrid : public Grid
	{
	public:
		RecoveryGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
