#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Weapons: mirrors Weapons.cpp's
	// BuildWeaponsMenu() in full - Globals, Tools, Ammu-Nation (see
	// AddAmmuNationRows()), Gun Van, Aimbot and Custom Weapons groups,
	// wired the same way SelfGrid wires Self > Main - see WeaponsGrid.cpp's
	// own predicate functions (IsCustomWeaponEnabled()/CustomWeaponType()/
	// IsGravityGunRow()/...) for how RenderCustomWeaponsMenu()'s several
	// levels of nested ConditionalItem/Group flatten onto individual
	// GridItemConditional rows.
	class WeaponsGrid : public Grid
	{
	public:
		WeaponsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
