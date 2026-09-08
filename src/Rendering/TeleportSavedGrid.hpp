#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class GridItemTextInput;

	// Content grid for Teleport > Saved: mirrors MenuTeleport.cpp's
	// RenderCustomTeleport() - a Category text field (also the single
	// source of truth for which category's locations are browsed below,
	// same shared-variable behaviour the original's own `category`
	// static has - selecting a category from the list below writes back
	// into this same field, matching the original exactly), a Location
	// Name field, a Save Current Location button, a GridItemSelectList
	// over SavedLocations::GetAllSavedLocations()'s own category names,
	// and a GridItemSelectList over the current category's own saved
	// locations - Shift-click deletes (through a MenuPopup confirm,
	// matching the original's own delete-confirmation popup), double-
	// click teleports, a plain click does nothing, exactly like the
	// original's own Selectable(..., ImGuiSelectableFlags_AllowDoubleClick)
	// handling.
	//
	// One simplification, documented at its own call site: the original
	// search box, once non-empty, searches every category at once
	// (SavedLocations::SavedLocationsFilteredList()), ignoring whichever
	// category is selected - GridItemSelectList's own built-in search
	// only narrows within whatever rows() returns, so here that's always
	// the selected category's own locations. Pick a category, then
	// search within it, rather than search-overrides-category.
	class TeleportSavedGrid : public Grid
	{
	public:
		TeleportSavedGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		// Raw pointers into this instance's own populated items - see
		// SavedVehiclesGrid's identical m_FileNameInput/m_NewFolderInput
		// pattern.
		GridItemTextInput* m_CategoryInput = nullptr;
		GridItemTextInput* m_LocationNameInput = nullptr;
	};
}
