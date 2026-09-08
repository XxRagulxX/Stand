#include "Rendering/VehicleSpawnNewGrid.hpp"

#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Scripting/FiberPool.hpp"
#include "Rendering/GridItemCommandToggle.hpp"
#include "Rendering/GridItemSelectList.hpp"
#include "Rendering/GridItemText.hpp"
#include "Util/Joaat.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"
#include "Rendering/Theme.hpp"
#include "Vehicle/Vehicle.hpp"
#include "Vehicle/Vehicles.hpp"

#include <unordered_map>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;

		// SpawnVehicle.cpp's own vehicleNames cache, parallel to
		// g_VehicleHashes (Vehicles.hpp) by index - built once via
		// FiberPool (the native calls below only work on the game's own
		// fiber) rather than inline the first time it's needed, same
		// "static bool init = [] { FiberPool::queueJob(...); return
		// true; }()" pattern the original ImGuiItem lambda used, just
		// hoisted out of a per-frame callback into its own function
		// GridItemSelectList's rows() can call every frame instead.
		std::vector<std::string> g_VehicleNames;

		void EnsureVehicleNamesLoaded()
		{
			static bool started = [] {
				FiberPool::queueJob([] {
					std::unordered_map<std::string, int> nameCount;

					for (auto& veh : g_VehicleHashes)
					{
						std::string gxt = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(veh);
						std::string display = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(gxt.c_str());

						int& count = nameCount[display == "NULL" ? gxt : display];
						std::string finalName = display == "NULL" ? gxt : display;
						if (count > 0)
							finalName += " " + std::to_string(count + 1);
						++count;

						std::string maker = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(VEHICLE::GET_MAKE_NAME_FROM_VEHICLE_MODEL(veh));
						if (maker != "NULL")
							finalName = maker + " " + finalName;

						g_VehicleNames.push_back(finalName);
					}
				});
				return true;
			}();
		}

		// GridItemSelectList's rows() callback - see EnsureVehicleNamesLoaded()
		// above for why this can be empty for a moment right after
		// startup (GridItemSelectList's own generic "No results" message
		// covers that transient state; the original showed a dedicated
		// "Natives not cached yet." line instead, not worth a special
		// case here for something that only shows for a moment).
		std::vector<std::string> VehicleRows()
		{
			EnsureVehicleNamesLoaded();
			return g_VehicleNames;
		}

		// index is into g_VehicleHashes (rows() returns g_VehicleNames,
		// the exact same length in the exact same order) - see
		// GridItemSelectList's own class comment for why onSelect always
		// gets an index into rows()'s own result rather than the
		// filtered list.
		void OnSelectVehicle(size_t index, const std::string&, bool, bool, bool)
		{
			if (index >= g_VehicleHashes.size())
				return;

			auto hash = g_VehicleHashes[index];
			FiberPool::queueJob([hash] {
				auto handle = Vehicle::Create(hash, Vehicle::GetSpawnLocRelToPed(Self::GetPed().GetHandle(), hash), Self::GetPed().GetHeading());

				auto* spawnInside = Commands::GetCommand<CommandToggleLegacy>("spawninsideveh"_J);
				if (spawnInside && spawnInside->GetState())
					Self::GetPed().SetInVehicle(handle);

				auto* spawnMaxed = Commands::GetCommand<CommandToggleLegacy>("spawnvehmaxed"_J);
				if (spawnMaxed && spawnMaxed->GetState())
					handle.Upgrade();
			});
		}
	}

	// Origin sits right below where VehicleSpawnGrid's own tab strip
	// draws (content origin + a 32-tall strip + 3 spacer) - see this
	// class's own header comment and GridTabbed's for why a tab target
	// uses Theme::GetTabbedContentOrigin() instead of the usual
	// Theme::GetContentOrigin() every other content Grid's own
	// constructor calls.
	VehicleSpawnNewGrid::VehicleSpawnNewGrid() :
	    Grid(Theme::GetTabbedContentOrigin(), 0)
	{
	}

	void VehicleSpawnNewGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		// Spawn (spawn Group) - the search box + result list ImGuiItem.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Spawn", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemSelectList>(Theme::kContentWidth, "Search", VehicleRows, OnSelectVehicle));

		// Settings (settings Group) - both unconditional.
		items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "Settings", Theme::kText));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "spawninsideveh"_J));
		items_draft.push_back(std::make_unique<GridItemCommandToggle>(Theme::kContentWidth, kItemH, "spawnvehmaxed"_J));
	}
}
