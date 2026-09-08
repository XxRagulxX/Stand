#pragma once
#include "Rendering/Grid.hpp"

#include <cstdint>

namespace Stand
{
	struct SavedPlayerData;
}

namespace Stand::Rendering
{
	class GridItemTextInput;

	// Content grid for Network > Saved Players: mirrors
	// MenuSavedPlayers.cpp's BuildSavedPlayersMenu() - a search-list of
	// every SavedPlayers::GetSavedPlayers() entry, an editor pane for
	// whichever one is selected (name/Rockstar Id fields, a Track Player
	// toggle, live session-state readout, Join/Save/Remove), an Add New
	// Player form, and the Tracking group's notification toggles.
	//
	// Two simplifications from the original, both documented at their
	// own call sites: no per-row coloured status circle (folded into a
	// bracketed state label in the row text instead - GridItemSelectList
	// only renders plain text rows); and the player list is a single,
	// name-sorted list rather than the original's three-tier grouping by
	// game state (public/other/invalid) - sorting by name also fixes
	// GetSavedPlayers() being an unordered_map, whose own iteration
	// order isn't guaranteed stable between GridItemSelectList's several
	// rows() calls per click otherwise (see that class's own class
	// comment on why rows() gets called more than once per interaction).
	class SavedPlayersGrid : public Grid
	{
	public:
		SavedPlayersGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		// nullptr if nothing's selected (m_SelectedRid == 0) or the
		// selection no longer resolves (removed elsewhere) - every info
		// row/editor field's own live callback checks this rather than
		// re-deriving the same lookup itself.
		[[nodiscard]] SavedPlayerData* SelectedPlayerData() const;

		std::uint64_t m_SelectedRid = 0;

		// Raw pointers into this instance's own populated items - see
		// SavedVehiclesGrid's identical m_FileNameInput/m_NewFolderInput
		// pattern.
		GridItemTextInput* m_NameInput = nullptr;
		GridItemTextInput* m_RidInput = nullptr;
		GridItemTextInput* m_AddUsernameInput = nullptr;
	};
}
