#pragma once
#include "Config/SavedVariables.hpp"
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	class GridItemTextInput;

	// Content grid for Debug > Globals: mirrors Core/Globals.cpp's
	// BuildGlobalsMenu() - an editor for one live SavedGlobal (m_CurGlobal,
	// same role as the original's own function-local static curGlobal) via
	// the shared AddSavedVariableDefinitionRows()/AddSavedVariableValueEditRows()
	// (SavedVariableGridRows.hpp - also used by LocalsGrid, same sharing
	// as the original's own DrawVariable.cpp), plus a saved-globals list
	// (name and a live value preview folded into the same row - see
	// SavedVariableGridRows.hpp's own comment on FormatSavedVariablePreview
	// for why there's one list here, not the original's two side by side)
	// and Save/Delete.
	//
	// Whenever m_CurGlobal's Type or appendage list actually changes
	// (editing the definition, loading a different saved global, or
	// Delete's own reset back to a blank SavedGlobal), the row(s)
	// AddSavedVariableDefinitionRows()/AddSavedVariableValueEditRows()
	// would build change too - each such action calls invalidate()
	// directly (via the onShapeChanged callback, or straight from this
	// class's own button/select callbacks) to repopulate, rather than a
	// per-frame Sync-style check the way e.g. PlayersGrid's own
	// SyncSelection() does, since every one of these changes is already a
	// discrete user action this class owns the callback for.
	class GlobalsGrid : public Grid
	{
	public:
		GlobalsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		SavedGlobal m_CurGlobal{};
		GridItemTextInput* m_NameInput = nullptr;
	};
}
