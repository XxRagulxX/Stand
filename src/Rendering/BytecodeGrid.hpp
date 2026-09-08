#pragma once
#include "Rendering/Grid.hpp"

#include <cstdint>
#include <vector>

namespace rage
{
	class scrProgram;
}

namespace Stand::Rendering
{
	class GridItemHexRow;

	// Content grid for Debug > Scripts > Threads > Bytecode: mirrors
	// MenuScripts.cpp's RenderBytecode() - a "Jump to Offset" field/
	// button plus one GridItemHexRow per 16 bytes of the current
	// program's own bytecode (see that class's own header comment for
	// why a row is its own top-level GridItem here, unlike the
	// original's ImGuiListClipper-virtualized inline loop).
	//
	// A separate page (reached through ThreadsGrid's own "Bytecode"
	// folder row) rather than the original's inline, collapsible
	// TreeNode - a large script's bytecode is thousands of rows, which
	// would otherwise bury Info/Kill/etc. under it on the same page
	// every time it's expanded.
	//
	// Owned directly as a ThreadsGrid member (not a file-scope static
	// the way most content Grids are - see SelfGrid.cpp's own note on
	// WeaponsGrid for that usual convention) since its content is
	// intrinsically tied to whichever thread ThreadsGrid currently has
	// selected, not an independent top-level category: ThreadsGrid calls
	// SetProgram() whenever its own selection changes.
	class BytecodeGrid : public Grid
	{
	public:
		BytecodeGrid();

		// Repoints this page at a different program (or none - pass
		// nullptr) and invalidates if it actually changed, so the next
		// draw rebuilds every row against the new program. A no-op if
		// program is already the one currently set - avoids discarding
		// this page's own scroll position/focus on every ThreadsGrid
		// repopulate that doesn't actually change which program is
		// selected.
		void SetProgram(rage::scrProgram* program);

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		rage::scrProgram* m_Program = nullptr;

		// Non-owning (this Grid's own item list owns the actual
		// objects) - kept so "Jump to Offset" can resolve an offset
		// straight to the row that contains it, same pattern as
		// MiscGrid's own m_InteriorIndexStepper.
		std::vector<GridItemHexRow*> m_Rows;
	};
}
