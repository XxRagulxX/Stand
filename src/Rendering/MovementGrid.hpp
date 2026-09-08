#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Movement - real Stand's own category
	// (CommandTabSelf.cpp), not this project's old flat "Movement"
	// section (a leftover of the classic StandEnhanced menu's own grouping,
	// unrelated to Stand's real tree). See its own populate() for
	// exactly what's here vs. still missing.
	class MovementGrid : public Grid
	{
	public:
		MovementGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
