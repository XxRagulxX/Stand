#include "Rendering/VehicleGrid.hpp"

#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/SavedVehiclesGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/VehicleEditorGrid.hpp"
#include "Rendering/VehicleSpawnGrid.hpp"

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// Owned here rather than in MenuGrid.cpp - see SelfGrid.cpp's
		// identical note about WeaponsGrid.
		VehicleSpawnGrid g_SpawnContent{};
		VehicleEditorGrid g_VehicleEditorContent{};
		SavedVehiclesGrid g_SavedVehiclesContent{};

		// autodrivespeed/autodrivestyle's own MenuVehicle.cpp gate reads
		// two BoolCommands (autodrive OR npcautodrive), so - same as
		// SelfGrid.cpp's ShouldClearOrSetWanted() - this needs
		// GridItemConditional's std::function<bool()> overload rather
		// than its plain joaat_t one.
		bool IsAutoDriveEnabled()
		{
			auto* playerAutoDrive = Commands::GetCommand<CommandToggleLegacy>("autodrive"_J);
			auto* npcAutoDrive = Commands::GetCommand<CommandToggleLegacy>("npcautodrive"_J);
			return (playerAutoDrive && playerAutoDrive->GetState()) || (npcAutoDrive && npcAutoDrive->GetState());
		}
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
	VehicleGrid::VehicleGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void VehicleGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Vehicle's other categories (BuildSpawnVehicleMenu()/
		// BuildVehicleEditorMenu()/BuildSavedVehiclesMenu()) - grouped at
		// the very top of the whole list rather than at the bottom, so a
		// category is always reachable before the plain toggles below.
		// All three now have their own content Grid.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Spawn", &g_SpawnContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Vehicle Editor", &g_VehicleEditorContent));
		items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Saved Vehicles", &g_SavedVehiclesContent));

		// Globals (MenuVehicle.cpp's globalsGroup) - watchCondition()
		// (not GridItemConditional) so boostbehavior/autodrivespeed/
		// autodrivestyle don't reserve their own layout slot while
		// hidden - see Grid::watchCondition()'s own doc comment. Combined
		// autodrive-OR-npcautodrive condition needs the
		// std::function<bool()> overload (IsAutoDriveEnabled() above),
		// same as before.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Globals", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "vehiclegodmode"_J, "Godmode"));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "keepfixed"_J, "Keep Fixed"));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "hornboost"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "modifyboostbehavior"_J));
		if (watchCondition("modifyboostbehavior"_J))
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "boostbehavior"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "autodrive"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "npcautodrive"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "autodrivehud"_J));
		if (watchCondition(IsAutoDriveEnabled))
			items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "autodrivespeed"_J));
		if (watchCondition(IsAutoDriveEnabled))
			items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "autodrivestyle"_J));

		// Tools (toolsGroup) - all plain CommandItem buttons.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Tools", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "enterlastvehicle"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "repairvehicle"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "fixallvehicles"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "callmechanic"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "requestpv"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "despawnpv"_J));
		items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "savepersonalvehicle"_J));

		// Misc (miscGroup) - every item here is an unconditional
		// BoolCommandItem in the original, so all six map directly onto
		// GridItemCommandToggle.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Misc", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "speedometer"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "seatbelt"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "lowervehiclestance"_J, "Lower Stance"));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "allowhatsinvehicles"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "lsccustomsbypass"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "dlcvehicles"_J));
	}
}
