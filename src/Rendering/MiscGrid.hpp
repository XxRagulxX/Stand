#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class GridItemIntStepper;
	class GridItemToggle;

	// Content grid for Debug's root (its Misc category's items, plus a
	// GridItemFolder row for each of Debug's other categories - Globals/
	// Locals/Scripts, each now its own real content Grid): mirrors
	// Misc.cpp's existing ImGui items, wired to their actual commands/
	// actions rather than placeholder state. Owned
	// and shown by MenuGrid (via MenuNavigation), which also owns the
	// header/sidebar chrome this used to include itself.
	//
	// Not a full replacement of Misc's ImGui rendering yet: there's no
	// keyboard text-entry system, so interiorIndex/Team use stepper
	// buttons (GridItemIntStepper) instead of free-form ImGui::InputInt
	// text fields. The existing ImGui Misc category is untouched and
	// still available as the free-form-entry alternative.
	class MiscGrid : public Grid
	{
	public:
		MiscGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		// Non-owning: the Grid's own item list owns the actual objects. Kept so the
		// DoTeleport/DoTeamSwap button callbacks can read their current
		// values at click time - mirrors Misc.cpp's own function-local
		// static int/bool captured by its ImGui button lambdas.
		GridItemIntStepper* m_InteriorIndexStepper = nullptr;
		GridItemToggle* m_EnterOwnerInteriorToggle = nullptr;
		GridItemIntStepper* m_TeamStepper = nullptr;
	};
}
