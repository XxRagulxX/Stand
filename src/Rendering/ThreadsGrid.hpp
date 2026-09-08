#pragma once
#include "Rendering/BytecodeGrid.hpp"
#include "Rendering/Grid.hpp"

namespace rage
{
	class scrThread;
	class scrProgram;
}

namespace Stand::Rendering
{
	// Content grid for Debug > Scripts > Threads: mirrors
	// MenuScripts.cpp's own "Threads" TabItem - a live thread picker
	// (GridItemSelectList over Pointers.ScriptThreads, filtered exactly
	// like the original's own combo), then, once one resolves to a valid
	// program, a State cycler, Kill/Log Labels, an Info block, and a
	// "Bytecode" folder leading to its own page (BytecodeGrid - see that
	// class's own header comment for why it's a separate page rather
	// than the original's inline collapsible TreeNode).
	//
	// A selected thread can die on its own, with no user action driving
	// it - unlike every other "shape changes on a discrete action" case
	// in this porting effort (GlobalsGrid/LocalsGrid/StatEditorGrid),
	// this needs the same per-frame SyncThread() check PlayersGrid's own
	// SyncSelection() does, called from draw()/drawText()/findItemAt()
	// before delegating to Grid's own, invalidating (and resetting back
	// to "no thread selected", same as the original's own per-frame
	// validity check) the moment the current selection stops being
	// valid.
	class ThreadsGrid : public Grid
	{
	public:
		ThreadsGrid();

		void draw() override;
		void drawText() override;
		GridItem* findItemAt(int16_t cursorX, int16_t cursorY) override;

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		void SyncThread();
		static bool IsThreadStillValid(rage::scrThread* thread, rage::scrProgram* program);

		rage::scrThread* m_CurThread = nullptr;
		rage::scrProgram* m_CurProgram = nullptr;

		// Owned directly rather than as a file-scope static - see
		// BytecodeGrid's own header comment for why.
		BytecodeGrid m_BytecodeContent{};
	};
}
