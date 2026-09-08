#include "Rendering/MiscGrid.hpp"

#include "Scripting/FiberPool.hpp"
#include "Rendering/GlobalsGrid.hpp"
#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemIntStepper.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemToggle.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/LocalsGrid.hpp"
#include "Scripting/Natives.hpp"
#include "Network/ScriptEvent.hpp"
#include "Rendering/ScriptsGrid.hpp"
#include "Commands/Widgets/CommandStandWidgetsTest5.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/StandWidgetsTestGrid3.hpp"
#include "Rendering/StandWidgetsTestGrid4.hpp"
#include "Scripting/ScriptFunction.hpp"
#include "World/Self.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;

		// Owned here rather than in MenuGrid.cpp - see SelfGrid.cpp's
		// identical note about WeaponsGrid.
		GlobalsGrid g_GlobalsContent{};
		LocalsGrid g_LocalsContent{};
		ScriptsGrid g_ScriptsContent{};
		StandWidgetsTestGrid3 g_StandWidgetsTest3Content{};
		StandWidgetsTestGrid4 g_StandWidgetsTest4Content{};
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
	MiscGrid::MiscGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void MiscGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Debug's other categories (BuildGlobalsMenu()/BuildLocalsMenu()/
		// BuildScriptsMenu()) - all three now have their own content
		// Grid, grouped at the very top of the whole list rather than at
		// the bottom, so a category is always reachable before the plain
		// items below.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, Theme::kContentItemHeight, "Globals", &g_GlobalsContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, Theme::kContentItemHeight, "Locals", &g_LocalsContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, Theme::kContentItemHeight, "Scripts", &g_ScriptsContent));

		// Test-only page for CommandToggleBitflag (see
		// CommandStandWidgetsTest3.cpp's own class comment) - delete
		// this row (and everything it references) once confirmed
		// working in-game.
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, Theme::kContentItemHeight, "Stand Widgets Test 3", &g_StandWidgetsTest3Content));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, Theme::kContentItemHeight, "Stand Widgets Test 4", &g_StandWidgetsTest4Content));

		// Phase 1 proof-of-concept for the Grid bridge onto real Stand's
		// own Command/CommandList/CommandToggle tree (Commands/Widgets/,
		// Rendering/GridItemStandCommand.hpp/GridStandCommandList.hpp) -
		// a real Stand::CommandToggle (CommandGod, see
		// CommandStandWidgetsTest5.cpp), rendered/clicked entirely
		// through that tree rather than this project's own parallel
		// Command/CommandToggle. Points directly at
		// GridStandCommandList::GetOrCreate() (the real bridge Grid,
		// not a hand-written test page) so this exercises the actual
		// thing a future migrated feature would use. Delete this row
		// (and everything CommandStandWidgetsTest5.hpp's own comment
		// references) once the real batch-by-batch migration is
		// underway.
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth,
		    Theme::kContentItemHeight,
		    "Stand Tree Test (Phase 1)",
		    &GridStandCommandList::GetOrCreate(&Features::GetStandTreeTestRoot())));

		// "Network Bail" isn't a registered Command in src/Misc.cpp (it's
		// an inline ImGui button + FiberPool job) - reused verbatim as a
		// GridItemButton action callback rather than wrapping it in a
		// Command just for this.
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, Theme::kContentItemHeight, "Network Bail", [] {
			FiberPool::queueJob([] {
				NETWORK::NETWORK_BAIL(0, 24, 0);
			});
		}));

		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, Theme::kContentItemHeight, "dumpdatahash"_J));

		// DoTeleport: interiorIndex stepper + enterOwnerInterior toggle,
		// same default values (0, false) as Misc.cpp's own function-local
		// statics. The button reads both at click time and sends the
		// exact same SCRIPT_EVENT_SEND_TO_INTERIOR Misc.cpp's DoTeleport
		// button does.
		auto interiorIndexStepper = std::make_unique<GridItemIntStepper>(Theme::kContentWidth, Theme::kContentItemHeight, "interiorIndex", 0, 0, 999);
		m_InteriorIndexStepper = interiorIndexStepper.get();
		items_draft.push_back(std::move(interiorIndexStepper));

		auto enterOwnerInteriorToggle = std::make_unique<GridItemToggle>(Theme::kContentWidth, Theme::kContentItemHeight, "enterOwnerInterior", false);
		m_EnterOwnerInteriorToggle = enterOwnerInteriorToggle.get();
		items_draft.push_back(std::move(enterOwnerInteriorToggle));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, Theme::kContentItemHeight, "DoTeleport", [this] {
			const int interiorIndex = m_InteriorIndexStepper ? m_InteriorIndexStepper->GetValue() : 0;
			const bool enterOwnerInterior = m_EnterOwnerInteriorToggle && m_EnterOwnerInteriorToggle->GetState();

			FiberPool::queueJob([interiorIndex, enterOwnerInterior] {
				SCRIPT_EVENT_SEND_TO_INTERIOR message;
				message.Interior = interiorIndex;
				message.EnterOwnerInterior = enterOwnerInterior;
				message.GoonsOnly = false;
				message.InstanceId = 0;
				message.SubInstanceId = -1;
				message.Owner = Self::GetPlayer().GetId();
				message.Distance = 99999;
				message.Position = {0, 0, 0};

				message.SetAllPlayers();
				message.Send();
			});
		}));

		// fm_mission_controller DoTeamSwap: Team stepper, same default (0)
		// as Misc.cpp's own function-local static int team.
		auto teamStepper = std::make_unique<GridItemIntStepper>(Theme::kContentWidth, Theme::kContentItemHeight, "Team", 0, 0, 8);
		m_TeamStepper = teamStepper.get();
		items_draft.push_back(std::move(teamStepper));

		// Display label shortened from Misc.cpp's literal button text
		// ("fm_mission_controller DoTeamSwap") - it overflows this panel's
		// width since GridItemButton has no text-wrapping yet. The action
		// underneath (the ScriptFunction call below) is unchanged.
		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, Theme::kContentItemHeight, "DoTeamSwap", [this] {
			const int team = m_TeamStepper ? m_TeamStepper->GetValue() : 0;

			FiberPool::queueJob([team] {
				static ScriptFunction DoTeamSwap("fm_mission_controller"_J, ScriptPointer("DoTeamSwap", "2D 02 04 00 00 38 00 50"));
				DoTeamSwap.Call<void>(team, true);
			});
		}));
	}
}
