#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Main: mirrors MenuSelf.cpp's Globals, Tools,
	// Special Ability, Wanted and Movement groups in full. Wanted's
	// two-level ConditionalItem nesting (clearWanted/setWanted groups,
	// each itself containing a further-conditional row) is flattened
	// into per-row conditions - see SelfGrid.cpp's own comment on
	// ShouldClearOrSetWanted() for the combined-AND case neither
	// CommandToggle alone can express.
	//
	// Any toggle that owns one or more dependent options of its own
	// (invis, mpspecialability, superrun, noclip, freecam, levitate)
	// gets a real folder page instead of leaving them flat as
	// GridItemConditional rows in this list - a hidden conditional row
	// still reserves its own layout slot (see GridItemConditional's own
	// class comment), so a toggle with dependent options left blank gaps
	// here while off. See InvisibilityGrid/MpSpecialAbilityGrid/
	// SuperRunGrid/NoclipGrid/FreecamGrid/LevitationGrid.hpp for what
	// each one covers.
	//
	// Also the Self submenu's root as far as MenuGrid/MenuNavigation are
	// concerned: Populate() appends a GridItemFolder row for each of
	// Self's other categories (Weapons, Outfit Editor) too, matching how
	// Stand's own menu nests a category inside another's list instead of
	// a tab/sidebar entry. Both folders point at a real content Grid
	// (WeaponsGrid/OutfitEditorGrid, owned here since nothing else
	// reaches them) - see each one's own header for what it covers.
	//
	// Every one of the above (Weapons, Outfit Editor, and each toggle's
	// own folder) is grouped together into one "Categories" section at
	// the very top of the whole list, ahead of every other section -
	// not just within its own section - so a feature that has a folder
	// is always reachable before any of this page's plain checkboxes.
	class SelfGrid : public Grid
	{
	public:
		SelfGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
