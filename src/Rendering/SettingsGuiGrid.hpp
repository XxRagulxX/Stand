#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Content grid for Settings > GUI: mirrors MenuSettings.cpp's gui
	// Category in full - unloadmenu and clearchat (plain CommandItem
	// buttons), uiStyle's styleselector (a ListCommandItem), overlay's
	// own toggle, and overlayfps/overlaypos/overlaylock (all three
	// ConditionalItems gated on it, now that GridItemConditional exists).
	class SettingsGuiGrid : public Grid
	{
	public:
		SettingsGuiGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
