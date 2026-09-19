#include "Commands/Vehicle/Spawn/CommandSpawnBlips.hpp"

#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Scripting/Natives.hpp"
#include "Vehicle/SpawnedVehicleMgr.hpp"

namespace Stand::Features
{
    namespace
    {
        void RemoveEntryBlip(SpawnedVehicleMgr::Entry& entry)
        {
            if (entry.blip && HUD::DOES_BLIP_EXIST(entry.blip))
                HUD::REMOVE_BLIP(&entry.blip);
            entry.blip = 0;
        }

        static StandWidgets::CommandLambdaAction s_ClearBlips{
            "clearspawnedblips",
            "Remove Blips",
            "Removes all blips from spawned vehicles.",
            [] { ClearAllSpawnedBlips(); }
        };
    }

    void AddSpawnedVehicleBlip(int vehicleHandle)
    {
        auto& tab = GetCommandTabSpawnSettings().spawnBlips;
        if (!tab->enabled->m_on)
            return;

        if (tab->latestOnly->m_on)
        {
            for (auto& entry : SpawnedVehicleMgr::s_Vehicles)
                RemoveEntryBlip(entry);
        }

        for (auto& entry : SpawnedVehicleMgr::s_Vehicles)
        {
            if (entry.handle != vehicleHandle)
                continue;

            const int blip = HUD::ADD_BLIP_FOR_ENTITY(vehicleHandle);
            HUD::SET_BLIP_COLOUR(blip, tab->colour->value);
            HUD::SET_BLIP_SCALE(blip, tab->scale->getFloatValue());
            entry.blip = blip;
            break;
        }
    }

    void ClearAllSpawnedBlips()
    {
        for (auto& entry : SpawnedVehicleMgr::s_Vehicles)
            RemoveEntryBlip(entry);
    }
}
