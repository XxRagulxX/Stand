#pragma once
#include "Commands/Widgets/CommandList.hpp"

namespace Stand::Features
{
	// Test-only root for the Stand Command/CommandList/CommandToggle
	// bridge (Rendering/GridItemStandCommand.hpp, Rendering/
	// GridStandCommandList.hpp) - a single real Stand::CommandList
	// holding one real Stand::CommandToggle child (CommandGod, in
	// CommandStandWidgetsTest5.cpp), proving the bridge actually
	// renders/clicks/toggles in the real menu before this project starts
	// migrating real features onto this foundation batch by batch (see
	// this project's own recent decision to adopt real Stand's own
	// Command/CommandList/CommandToggle/CommandSlider tree,
	// Commands/Widgets/, as the foundation going forward instead of this
	// project's own parallel Command/CommandToggle/CommandSlider - kept
	// running side by side, unmodified, until the migration is complete).
	// Delete this whole file (and its .cpp, and the folder row in
	// MiscGrid.cpp) once that migration is underway for real.
	Stand::CommandList& GetStandTreeTestRoot();
}
