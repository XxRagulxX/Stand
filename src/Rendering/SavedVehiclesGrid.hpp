#pragma once
#include "Rendering/Grid.hpp"

#include <string>
#include <vector>

namespace Stand::Rendering
{
	class GridItemTextInput;

	// Content grid for Vehicle > Saved Vehicles: mirrors SavedVehicles.cpp's
	// BuildSavedVehiclesMenu() - folder browsing (a GridItemSelectList
	// over the live SavedVehicles::RefreshList() folder list, "Root"
	// prepended), a search-list of saved vehicles in the current folder
	// (spawning through a MenuPopup confirm, same as the original's own
	// "Are you sure you want to spawn X" modal), Save (+ Populate Name)
	// and Refresh List buttons.
	//
	// One simplification, documented at its own call site: the original
	// only shows its "New Folder" name field while browsing Root (saving
	// into an existing folder doesn't need one) - this always shows it,
	// and Save only actually uses it when the current folder is Root,
	// rather than repopulating the page every time the folder changes
	// just to show/hide one row.
	class SavedVehiclesGrid : public Grid
	{
	public:
		SavedVehiclesGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		void RefreshList();

		std::string m_Folder; // "" means Root, same convention SavedVehicles::RefreshList() itself uses
		std::vector<std::string> m_Folders;
		std::vector<std::string> m_Files;

		// Raw pointers into this instance's own populated items (see
		// PlayersGrid's identical m_Sidebar/m_Header pattern) - read by
		// the Save button's own action, written by the Populate Name
		// button's.
		GridItemTextInput* m_FileNameInput = nullptr;
		GridItemTextInput* m_NewFolderInput = nullptr;
	};
}
