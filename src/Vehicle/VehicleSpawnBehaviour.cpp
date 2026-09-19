#include "Vehicle/VehicleSpawnBehaviour.hpp"

#include "Commands/Vehicle/CommandTabVehicle.hpp"
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
    }

    void SpawnVehicleOnFoot(joaat_t hash, const std::string& name)
    {
        auto& tab = Features::GetCommandTabVehicle();

        if (tab.footDelete->m_on && g_FootPreviousHandle != 0)
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

        if (tab.footLikePV->m_on)
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
        else if (tab.footFront->m_on)
        {
            auto loc = Vehicle::GetSpawnLocRelToPed(Self::GetPed().GetHandle(), hash);
            spawnPos = { loc.x, loc.y, loc.z };
        }
        else
        {
            spawnPos = Self::GetPed().GetPosition();
        }

        if (tab.footAir->m_on &&
            (VEHICLE::IS_THIS_MODEL_A_HELI(hash) || VEHICLE::IS_THIS_MODEL_A_PLANE(hash)))
        {
            spawnPos.z += 30.f;
        }

        auto veh = Vehicle::Create(hash, spawnPos, heading);
        if (!veh.IsValid())
            return;

        if (tab.spawngod->m_on)
            ApplyGodMode(veh);

        SpawnedVehicleMgr::Add(veh.GetHandle(), name);
        g_FootPreviousHandle = veh.GetHandle();

        if (tab.footDrive->m_on)
        {
            Script::current()->yield(200);
            Self::GetPed().SetInVehicle(veh);
        }
    }

    void SpawnVehicleInVehicle(joaat_t hash, const std::string& name)
    {
        auto& tab = Features::GetCommandTabVehicle();

        const int pedHandle    = Self::GetPed().GetHandle();
        const int curVehHandle = PED::GET_VEHICLE_PED_IS_IN(pedHandle, false);

        if (tab.vehDelete->m_on && g_VehPreviousHandle != 0)
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
        if (tab.vehVelocity->m_on && curVehHandle != 0)
            curVelocity = Vehicle(curVehHandle).GetVelocity();

        float heading = Self::GetPed().GetHeading();
        rage::fvector3 spawnPos{};

        if (tab.vehLikePV->m_on)
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
        else if (tab.vehFront->m_on && curVehHandle != 0)
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

        if (tab.vehAir->m_on &&
            (VEHICLE::IS_THIS_MODEL_A_HELI(hash) || VEHICLE::IS_THIS_MODEL_A_PLANE(hash)))
        {
            spawnPos.z += 30.f;
        }

        auto veh = Vehicle::Create(hash, spawnPos, heading);
        if (!veh.IsValid())
            return;

        if (tab.vehVelocity->m_on)
            veh.SetVelocity(curVelocity);

        if (tab.spawngod->m_on)
            ApplyGodMode(veh);

        SpawnedVehicleMgr::Add(veh.GetHandle(), name);
        g_VehPreviousHandle = veh.GetHandle();

        if (tab.vehDrive->m_on)
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
