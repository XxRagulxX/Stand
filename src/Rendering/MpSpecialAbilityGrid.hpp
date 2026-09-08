#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Self > Main > Special Ability > MP Special
	// Ability: the mpspecialability toggle itself plus its one
	// dependent option (selspecialability, the ability-type dropdown,
	// gated on mpspecialability being on) - see InvisibilityGrid.hpp's
	// own comment for why this moved out of SelfGrid's flat list into
	// its own folder page. infspecialability has no dependent option of
	// its own, so it stays a plain toggle in SelfGrid's Special Ability
	// section rather than getting a folder here too.
	class MpSpecialAbilityGrid : public Grid
	{
	public:
		MpSpecialAbilityGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
