#include "Rendering/MovementGrid.hpp"

#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/LevitationGrid.hpp"
#include "Rendering/SuperRunGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kItemH = Theme::kContentItemHeight;

		// Owned here rather than in SelfGrid.cpp - only ever reached
		// through their own folder row below now.
		LevitationGrid g_LevitationContent{};
		SuperRunGrid g_SuperRunContent{};
	}

	// Origin/spacer match every other content Grid's - see SelfGrid.cpp's
	// identical comment. Same origin regardless of nesting depth (Self >
	// Movement is one level deeper than Self's own root) - MenuNavigation
	// only ever shows one Grid at a time, in the same content area, so
	// nesting doesn't change where this draws.
	MovementGrid::MovementGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void MovementGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// In real Stand's own CommandTabSelf.cpp order. Levitation/Super
		// Run are folders (real Stand's own dependent options make both
		// too big for a flat row - see LevitationGrid.hpp/SuperRunGrid.hpp's
		// own comments). Still missing, deliberately (not guessed at -
		// see each new command's own file for the specific infrastructure
		// gap): Friction (camera-relative input helpers), Graceful
		// Landing/Move Freedom (Stand's own hooking/ControlMgr), Drunk
		// Mode (steering-drift timer), Fly/Floppy Mode (each needs its
		// own folder page, not built yet).
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Levitation", &g_LevitationContent));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "walkspeed"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "superjump"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "waterwalk"_J));
		items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "swimspeed"_J));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Super Run", &g_SuperRunContent));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "airwalk"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "airswim"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "tennismode"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "ghostmode"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "nocollision"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "selffreeze"_J));
	}
}
