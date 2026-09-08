#include "Rendering/RecoveryHeistsGrid.hpp"

#include "Rendering/ApartmentHeistGrid.hpp"
#include "Rendering/CayoPericoHeistGrid.hpp"
#include "Rendering/DiamondCasinoHeistGrid.hpp"
#include "Rendering/DoomsdayHeistGrid.hpp"
#include "Rendering/KortzCenterHeistGrid.hpp"
#include "Rendering/MissionsGrid.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		// Owned here rather than in MenuGrid.cpp - see SelfGrid.cpp's
		// identical note about WeaponsGrid. Each already hardcodes its
		// own origin below where this Grid's own tab strip draws - see
		// their own class comments.
		ApartmentHeistGrid g_ApartmentContent{};
		DiamondCasinoHeistGrid g_DiamondCasinoContent{};
		CayoPericoHeistGrid g_CayoPericoContent{};
		DoomsdayHeistGrid g_DoomsdayContent{};
		MissionsGrid g_MissionsContent{};
		KortzCenterHeistGrid g_KortzCenterContent{};
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). This Grid's own item list is just
	// the tab strip below - everything else lives in whichever target is
	// active.
	RecoveryHeistsGrid::RecoveryHeistsGrid() :
	    GridTabbed(Theme::GetContentOrigin(), 3)
	{
	}

	void RecoveryHeistsGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		InitTabs(items_draft,
		    static_cast<int16_t>(Theme::kContentItemHeight),
		    {"Apartment Heist", "Diamond Casino Heist", "Cayo Perico Heist", "Doomsday Heist", "Missions", "Kortz Center Heist"},
		    {&g_ApartmentContent, &g_DiamondCasinoContent, &g_CayoPericoContent, &g_DoomsdayContent, &g_MissionsContent, &g_KortzCenterContent});
	}
}
