#pragma once
#include "Rendering/Grid.hpp"

namespace Stand::Rendering
{
	// Reachable from Settings > Debug > "Stand Widgets Test 3" -
	// exercises CommandToggleBitflag live, same treatment as the two rounds of
	// Commands/Widgets/ tests before it (see CommandStandWidgetsTest3.cpp's
	// own class comment). Delete this whole class (and its .cpp, and
	// CommandStandWidgetsTest3.cpp/.hpp, and the folder row in
	// MiscGrid.cpp) once it's confirmed working in-game.
	class StandWidgetsTestGrid3 : public Grid
	{
	public:
		StandWidgetsTestGrid3();

	protected:
		void populate(std::vector<std::unique_ptr<GridItem>>& items_draft) override;
	};
}
