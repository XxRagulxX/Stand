#pragma once
#include "Rendering/Grid.hpp"

#include <memory>

namespace Stand
{
	class LuaScript;
}

namespace Stand::Rendering
{
	// Content grid for Settings > Lua Scripts: mirrors LuaScripts.cpp's
	// BuildLuaScriptsMenu() - a search-list of every loaded script (name
	// only) plus every unloaded one (an "[Unloaded] " prefix stands in
	// for the original's own greyed-out text colour - GridItemSelectList
	// only renders plain text rows), and an editor pane for whichever
	// loaded script is selected (Pause/Resume - as two separately-gated
	// buttons rather than one relabelled one, since GridItemButton's own
	// label is fixed at construction - Reload, Unload). Every editor row
	// is gated on IsSelectionValid() (LuaScriptsGrid.cpp), which folds in
	// the original's own per-frame "did the selected script unload
	// underneath us" guard directly rather than needing a separate
	// invalidate()/repopulate cycle (see e.g. PlayersGrid's own
	// SyncSelection()) - nothing else here depends on m_SelectedScript
	// itself going back to null once that happens, only on whether the
	// editor rows should still show.
	//
	// Clicking an unloaded row loads it directly (LuaManager::LoadScript())
	// rather than deferring to a "load after this frame's loop" flag the
	// way the original's own ForAllUnloadedScripts callback body had to -
	// that flag existed only because loading from *inside* that specific
	// iteration callback deadlocks; GridItemSelectList's onSelect runs
	// from an entirely separate context (a mouse click), not from inside
	// a ForAllUnloadedScripts call, so the same restriction doesn't apply
	// here.
	class LuaScriptsGrid : public Grid
	{
	public:
		LuaScriptsGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		std::shared_ptr<LuaScript> m_SelectedScript;
	};
}
