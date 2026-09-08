#pragma once
#include "Rendering/GridTabbed.hpp"

namespace Stand::Rendering
{
	// Content grid for Recovery > Heists: mirrors HeistModifier.cpp's
	// BuildHeistModifierMenu() TabBarItem in full - Apartment/Diamond
	// Casino/Cayo Perico/Doomsday/Missions/Kortz Center Heist tabs, each
	// with its own content Grid.
	class RecoveryHeistsGrid : public GridTabbed
	{
	public:
		RecoveryHeistsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
