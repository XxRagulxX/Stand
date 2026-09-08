#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Main > Levitation: the levitate toggle
	// itself plus all of its dependent options (all gated on levitate
	// being on) - the real dedicated folder page SelfGrid.cpp used to
	// note as "a reasonable follow-up" while these lived flat in its own
	// list (see InvisibilityGrid.hpp's own comment for the general
	// reasoning: a hidden GridItemConditional row still reserves its own
	// layout slot, so a toggle with many dependent rows left blank gaps
	// where they'd be while off - by far the worst case of that here,
	// with ~15 of them).
	class LevitationGrid : public Grid
	{
	public:
		LevitationGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
