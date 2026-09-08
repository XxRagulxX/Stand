#include "Rendering/PlayersGrid.hpp"

#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemPlayerList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/InfoGrid.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/KickGrid.hpp"
#include "Network/Players.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/ToxicGrid.hpp"
#include "Rendering/TrollGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Owned here rather than in MenuGrid.cpp - see SelfGrid.cpp's
		// identical note about WeaponsGrid.
		InfoGrid g_InfoContent{};
		ToxicGrid g_ToxicContent{};
		TrollGrid g_TrollContent{};
		KickGrid g_KickContent{};
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Each item below specifies its own
	// width (Theme::kContentWidth) rather than the Grid itself, matching
	// Stand's real Grid - see Grid.hpp's class comment.
	PlayersGrid::PlayersGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void PlayersGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		m_LastHasSelection = Players::GetSelected().IsValid();

		// DrawPlayerList()'s own spectate toggle + player list - always
		// shown, regardless of selection (this is how you select someone
		// in the first place). No section header of its own - one
		// repeating "Players" while already inside the Players tab
		// (MenuGrid's own breadcrumb already reads "... > Players") is a
		// redundant label, not a distinguishing one - see SelfGrid.cpp's
		// identical comment on its own former "Self" header.
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "spectate"_J));
		items_draft.push_back(std::make_unique<GridItemPlayerList>(Theme::kContentWidth));

		// Every one of MenuPlayers.cpp's own categories - only once a
		// player is actually selected (see the class comment in
		// PlayersGrid.hpp for why). With nobody selected (including
		// because nobody's connected at all), the page is just the
		// toggle + list above - SyncSelection() re-populates as soon as
		// that changes.
		if (m_LastHasSelection)
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
			items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Info", &g_InfoContent));
			items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Troll", &g_TrollContent));
			items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Toxic", &g_ToxicContent));
			items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Kick", &g_KickContent));
		}
	}

	void PlayersGrid::SyncSelection()
	{
		const auto hasSelection = Players::GetSelected().IsValid();
		if (hasSelection != m_LastHasSelection)
			invalidate();
	}

	void PlayersGrid::draw()
	{
		SyncSelection();
		Grid::draw();
	}

	void PlayersGrid::drawText()
	{
		SyncSelection();
		Grid::drawText();
	}

	GridItem* PlayersGrid::findItemAt(int16_t cursorX, int16_t cursorY)
	{
		SyncSelection();
		return Grid::findItemAt(cursorX, cursorY);
	}
}
