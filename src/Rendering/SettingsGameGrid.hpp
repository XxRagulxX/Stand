#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Settings > Game: mirrors MenuSettings.cpp's Player
	// ESP, Ped ESP and Object ESP groups in full - every conditional row
	// is gated on the espdraw*/on toggle at the top of its own group
	// (espdrawplayers/espdrawpeds/espdrawobjects), via
	// GridItemConditional's plain joaat_t overload, same as everywhere
	// else in this pass. The name-colour/skeleton-colour/hash-colour
	// swatches use GridItemCommandColourCustom's AddColorCommandRows() the same
	// gated way WeaponsGrid's paintguncolor does - see that file's
	// AddConditionalColorCommandRows() for why a plain GridItemConditional
	// around one GridItem isn't enough there.
	class SettingsGameGrid : public Grid
	{
	public:
		SettingsGameGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
