#pragma once
#include "Rendering/Grid.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	// Content grid for Debug > Scripts > Start Script: mirrors
	// MenuScripts.cpp's own "Start Script" TabItem - a searchable script
	// name picker (GridItemSelectList over the shared scriptNames array),
	// a Stack Size cycler (GridItemLocalList over the same stackSizes
	// table the original's own combo used - file-static there, so
	// duplicated here rather than reached into, same as every other port
	// in this series), an Arg Count stepper with one Arg[i] row per
	// argument, Pause After Starting, and Start Script (plus Start
	// Session Script/Start Script With Event once a launcher script is
	// picked and a session is active).
	//
	// m_Args uses a std::vector<std::int64_t>, resized on every Arg
	// Count change - the same value-preserving resize the original's own
	// manual new[]/memcpy/delete[] did, just expressed with a container
	// that already does it. Each Arg[i] row is a plain GridItemIntStepper
	// narrowed to int32 range (no 64-bit stepper widget exists yet - same
	// documented tradeoff as SavedVariableGridRows.cpp's own INT64 case).
	//
	// Picking a script (from the list) resolves m_LauncherIndex right
	// there and invalidates - whether the two launcher-only buttons show
	// at all depends on it, so it's a shape change like everything else
	// in this porting effort that adds/removes rows. Changing Arg Count
	// invalidates for the same reason (it changes how many Arg[i] rows
	// exist).
	class StartScriptGrid : public Grid
	{
	public:
		StartScriptGrid();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;

	private:
		std::string m_SelectedScript;
		int m_StackSize = 0;
		int m_ArgCount = 0;
		std::vector<std::int64_t> m_Args;
		bool m_PauseAfterStarting = false;
		std::optional<int> m_LauncherIndex;
	};
}
