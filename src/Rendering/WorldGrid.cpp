#include "Rendering/WorldGrid.hpp"

#include "Rendering/GridItemCommandButton.hpp"
#include "Rendering/GridItemCommandSlider.hpp"
#include "Rendering/GridItemCommandListSelect.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemFolder.hpp"
#include "Rendering/GridItemStandCommand.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Rendering/SpawnPedGrid.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/WorldIPLsGrid.hpp"
#include "Commands/World/CommandTabWorld.hpp"

namespace Stand::Rendering
{
	namespace
	{
		// constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		// constexpr float kItemH = Theme::kContentItemHeight;

		// WorldIPLsGrid g_IPLsContent{};
		// SpawnPedGrid g_SpawnPedContent{};
	}

	WorldGrid::WorldGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void WorldGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		constexpr int16_t h = static_cast<int16_t>(Theme::kContentItemHeight);
		auto& worldTab = Features::GetCommandTabWorld();
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.atmosphere));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.water));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.editor));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.places));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.inhabitants));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.worldstate));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.enhancedopenworld));
		items_draft.push_back(std::make_unique<GridItemStandCommand>(Theme::kContentWidth, h, worldTab.dedsec));


		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Categories", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "Spawn Ped", &g_SpawnPedContent));
		// items_draft.push_back(std::make_unique<GridItemFolder>(Theme::kContentWidth, kItemH, "IPLs", &g_IPLsContent));

		// // Kill (killPeds) - both plain CommandItem buttons.
		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Kill", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "killallpeds"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "killallenemies"_J));

		// // Delete (deleteOpts) - all plain CommandItem buttons.
		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Delete", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "delpeds"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "delvehs"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "delobjs"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "delcams"_J));

		// // Bring (bringOpts) - all plain CommandItem buttons.
		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Bring", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "bringpeds"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "bringvehs"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "bringobjs"_J));

		// // Weather (weatherOpts) - setweather is shown only while
		// // forceweather is *off* (negate); watchCondition() (not
		// // GridItemConditional) so it doesn't reserve its own layout slot
		// // while hidden.
		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Weather", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemCommandListSelect>(Theme::kContentWidth, kItemH, "weather"_J));
		// if (watchCondition("forceweather"_J, true))
		// 	items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "setweather"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "forceweather"_J));

		// // Time (timeGroup) - hour/minute/second are all unconditional
		// // IntCommandItems, now that GridItemCommandSlider exists; Set/Freeze
		// // are a plain button and toggle.
		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Time", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "networktimehour"_J, "Hour"));
		// items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "networktimeminute"_J, "Minute"));
		// items_draft.push_back(std::make_unique<GridItemCommandSlider>(Theme::kContentWidth, kItemH, "networktimesecond"_J, "Second"));
		// items_draft.push_back(std::make_unique<GridItemCommandButton>(Theme::kContentWidth, kItemH, "setnetworktime"_J, "Set"));
		// items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "freezenetworktime"_J, "Freeze"));

		// // Other (otherOpts) - every item here is an unconditional
		// // BoolCommandItem in the original, so all five map directly onto
		// // GridItemCommandToggle.
		// items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Other", Theme::kText));
		// items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "pedsignore"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "PedRiotMode"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "CopsDispatch"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "enablecreatordevmode"_J));
		// items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "infiniteboundary"_J));
	}
}
