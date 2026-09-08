#pragma once
#include "Config/SavedVariables.hpp"
#include "Rendering/Grid.hpp"

#include <string>

namespace rage
{
	class scrThread;
}

namespace Stand::Rendering
{
	class GridItemTextInput;

	// Content grid for Debug > Locals: mirrors Scripting/Locals.cpp's
	// BuildLocalsMenu() - a Script name field resolving m_CurThread (same
	// role as the original's own function-local static curThread), then
	// (only once a thread is actually found - matching the original's own
	// early return) an editor for one live SavedLocal (m_CurLocal) via the
	// shared AddSavedVariableDefinitionRows()/AddSavedVariableValueEditRows()
	// (SavedVariableGridRows.hpp - also used by GlobalsGrid), plus a
	// saved-locals list filtered to the current script (name and a live
	// value preview folded into the same row - see
	// SavedVariableGridRows.hpp's own comment on FormatSavedVariablePreview)
	// and Save/Delete, both keyed on (name, script) like the original's
	// own SaveLocal()/DeleteLocal().
	//
	// Committing a new Script name resolves m_CurThread and resets
	// m_CurLocal to a blank SavedLocal for that script right there (the
	// original does the same reset per-frame, comparing
	// curThread->m_ScriptHash against curLocal.script - here it only ever
	// changes through this one commit action, so there's nothing to
	// poll), then invalidates - same reasoning as every other
	// shape-changing action in this class (see GlobalsGrid.hpp's own
	// header comment).
	class LocalsGrid : public Grid
	{
	public:
		LocalsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		SavedLocal m_CurLocal{};
		rage::scrThread* m_CurThread = nullptr;
		std::string m_ScriptName;
		GridItemTextInput* m_NameInput = nullptr;
	};
}
