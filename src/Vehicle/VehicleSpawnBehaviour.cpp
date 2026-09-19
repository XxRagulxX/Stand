#include "Vehicle/VehicleSpawnBehaviour.hpp"

#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnOnFoot.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnInVehicle.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnPlate.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnColours.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnBlips.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Vehicle/SpawnedVehicleMgr.hpp"
#include "Vehicle/Vehicle.hpp"
#include "World/Self.hpp"

namespace Stand
{
    namespace
    {
        int g_FootPreviousHandle = 0;
        int g_VehPreviousHandle  = 0;

        void ApplyGodMode(Vehicle veh)
        {
            const int h = veh.GetHandle();
            ENTITY::SET_ENTITY_INVINCIBLE(h, TRUE, FALSE);
            ENTITY::SET_ENTITY_PROOFS(h, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);
        }

        void ApplyColour(Vehicle veh)
        {
            if (!Features::GetCommandTabSpawnSettings().spawnColour->enabled->m_on)
                return;
            const int h = veh.GetHandle();
            const auto p = Features::GetSpawnPrimaryColour();
            const auto s = Features::GetSpawnSecondaryColour();
            VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(h,
                static_cast<int>(p.x * 255.f),
                static_cast<int>(p.y * 255.f),
                static_cast<int>(p.z * 255.f));
            VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(h,
                static_cast<int>(s.x * 255.f),
                static_cast<int>(s.y * 255.f),
                static_cast<int>(s.z * 255.f));
        }

        void ApplyTune(Vehicle veh, int tune)
        {
            if (tune == 0)
                return;

            const int h = veh.GetHandle();

            if (tune == 3)
            {
                veh.Upgrade();
                return;
            }

            VEHICLE::SET_VEHICLE_MOD_KIT(h, 0);
            VEHICLE::SET_VEHICLE_MOD(h, 11, VEHICLE::GET_NUM_VEHICLE_MODS(h, 11) - 1, false);
            VEHICLE::SET_VEHICLE_MOD(h, 12, VEHICLE::GET_NUM_VEHICLE_MODS(h, 12) - 1, false);
            VEHICLE::SET_VEHICLE_MOD(h, 13, VEHICLE::GET_NUM_VEHICLE_MODS(h, 13) - 1, false);
            VEHICLE::SET_VEHICLE_MOD(h, 16, VEHICLE::GET_NUM_VEHICLE_MODS(h, 16) - 1, false);
            VEHICLE::TOGGLE_VEHICLE_MOD(h, 18, TRUE);

            if (tune == 2)
                VEHICLE::SET_VEHICLE_MOD(h, 0, VEHICLE::GET_NUM_VEHICLE_MODS(h, 0) - 1, false);
        }
    }

    void SpawnVehicleOnFoot(joaat_t hash, const std::string& name)
    {
        auto& foot = Features::GetCommandTabSpawnOnFoot();
        auto& cfg  = Features::GetCommandTabSpawnSettings();

        if (foot.deleteprevious->m_on && g_FootPreviousHandle != 0)
        {
            Vehicle prev(g_FootPreviousHandle);
            if (prev.IsValid())
            {
                const auto model = prev.GetModel();
                prev.Delete();
                STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
                SpawnedVehicleMgr::Remove(g_FootPreviousHandle);
            }
            g_FootPreviousHandle = 0;
        }

        float heading = Self::GetPed().GetHeading();
        rage::fvector3 spawnPos{};

        if (foot.likepv->m_on)
        {
            const auto pedPos = Self::GetPed().GetPosition();
            Vector3 nodePos{0.f, 0.f, 0.f};
            float nodeHeading = 0.f;
            PATH::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(
                pedPos.x, pedPos.y, pedPos.z,
                &nodePos, &nodeHeading,
                1, 3.f, 0.f);
            spawnPos = { nodePos.x, nodePos.y, nodePos.z };
            heading = nodeHeading;
        }
        else if (foot.spawnfront->m_on)
        {
            auto loc = Vehicle::GetSpawnLocRelToPed(Self::GetPed().GetHandle(), hash);
            spawnPos = { loc.x, loc.y, loc.z };
        }
        else
        {
            spawnPos = Self::GetPed().GetPosition();
        }

        if (foot.spawnair->m_on &&
            (VEHICLE::IS_THIS_MODEL_A_HELI(hash) || VEHICLE::IS_THIS_MODEL_A_PLANE(hash)))
        {
            spawnPos.z += 30.f;
        }

        auto veh = Vehicle::Create(hash, spawnPos, heading);
        if (!veh.IsValid())
            return;

        if (cfg.spawngod->m_on)
            ApplyGodMode(veh);
        ApplyTune(veh, cfg.spawntune->value);
        ApplyColour(veh);

        const auto plateText = Features::GetSpawnPlateText();
        if (!plateText.empty())
            veh.SetPlateText(plateText);

        SpawnedVehicleMgr::Add(veh.GetHandle(), name);
        Features::AddSpawnedVehicleBlip(veh.GetHandle());
        g_FootPreviousHandle = veh.GetHandle();

        if (foot.drivespawned->m_on)
        {
            Script::current()->yield(200);
            Self::GetPed().SetInVehicle(veh);
        }
    }

    void SpawnVehicleInVehicle(joaat_t hash, const std::string& name)
    {
        auto& veh_tab = Features::GetCommandTabSpawnInVehicle();
        auto& cfg     = Features::GetCommandTabSpawnSettings();

        const int pedHandle    = Self::GetPed().GetHandle();
        const int curVehHandle = PED::GET_VEHICLE_PED_IS_IN(pedHandle, false);

        if (veh_tab.deleteprevious->m_on && g_VehPreviousHandle != 0)
        {
            Vehicle prev(g_VehPreviousHandle);
            if (prev.IsValid())
            {
                const auto model = prev.GetModel();
                prev.Delete();
                STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
                SpawnedVehicleMgr::Remove(g_VehPreviousHandle);
            }
            g_VehPreviousHandle = 0;
        }

        rage::fvector3 curVelocity{};
        if (veh_tab.keepvelocity->m_on && curVehHandle != 0)
            curVelocity = Vehicle(curVehHandle).GetVelocity();

        float heading = Self::GetPed().GetHeading();
        rage::fvector3 spawnPos{};

        if (veh_tab.likepv->m_on)
        {
            const auto pedPos = Self::GetPed().GetPosition();
            Vector3 nodePos{0.f, 0.f, 0.f};
            float nodeHeading = 0.f;
            PATH::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(
                pedPos.x, pedPos.y, pedPos.z,
                &nodePos, &nodeHeading,
                1, 3.f, 0.f);
            spawnPos = { nodePos.x, nodePos.y, nodePos.z };
            heading = nodeHeading;
        }
        else if (veh_tab.spawnfront->m_on && curVehHandle != 0)
        {
            Vector3 curMin{}, curMax{}, newMin{}, newMax{};
            MISC::GET_MODEL_DIMENSIONS(Vehicle(curVehHandle).GetModel(), &curMin, &curMax);
            MISC::GET_MODEL_DIMENSIONS(hash, &newMin, &newMax);
            const float spacing = (curMax.y - curMin.y) * 0.5f + (newMax.y - newMin.y) + 1.f;
            auto loc = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(curVehHandle, 0.f, spacing, 0.f);
            spawnPos = { loc.x, loc.y, loc.z };
        }
        else
        {
            spawnPos = Self::GetPed().GetPosition();
        }

        if (veh_tab.spawnair->m_on &&
            (VEHICLE::IS_THIS_MODEL_A_HELI(hash) || VEHICLE::IS_THIS_MODEL_A_PLANE(hash)))
        {
            spawnPos.z += 30.f;
        }

        auto veh = Vehicle::Create(hash, spawnPos, heading);
        if (!veh.IsValid())
            return;

        if (veh_tab.keepvelocity->m_on)
            veh.SetVelocity(curVelocity);

        if (cfg.spawngod->m_on)
            ApplyGodMode(veh);
        ApplyTune(veh, cfg.spawntune->value);
        ApplyColour(veh);

        const auto plateText = Features::GetSpawnPlateText();
        if (!plateText.empty())
            veh.SetPlateText(plateText);

        SpawnedVehicleMgr::Add(veh.GetHandle(), name);
        Features::AddSpawnedVehicleBlip(veh.GetHandle());
        g_VehPreviousHandle = veh.GetHandle();

        if (veh_tab.drivespawned->m_on)
        {
            Script::current()->yield(200);
            Self::GetPed().SetInVehicle(veh);
        }
    }

    void SpawnVehicle(joaat_t hash, const std::string& name)
    {
        if (PED::IS_PED_IN_ANY_VEHICLE(Self::GetPed().GetHandle(), false))
            SpawnVehicleInVehicle(hash, name);
        else
            SpawnVehicleOnFoot(hash, name);
    }
}
