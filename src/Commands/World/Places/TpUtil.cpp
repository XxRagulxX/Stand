#include "Commands/World/Places/TpUtil.hpp"

#include <cmath>
#include "Commands/World/Places/CommandTeleportParticle.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    float TpUtil::vDist2D(float x1, float y1, float x2, float y2)
    {
        float dx = x2 - x1, dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }

    int TpUtil::getPlayerVehicle()
    {
        int ped = PLAYER::GET_PLAYER_PED(-1);
        int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
        if (veh && ENTITY::DOES_ENTITY_EXIST(veh) && PED::IS_PED_IN_VEHICLE(ped, veh, false))
            return veh;
        return 0;
    }

    Vector3 TpUtil::getPlayerPos()
    {
        int ped = PLAYER::GET_PLAYER_PED(-1);
        int veh = getPlayerVehicle();
        return ENTITY::GET_ENTITY_COORDS(veh ? veh : ped, TRUE);
    }

    float TpUtil::getEntityHalfHeight(int entity)
    {
        Hash model = ENTITY::GET_ENTITY_MODEL(entity);
        Vector3 minDim{}, maxDim{};
        MISC::GET_MODEL_DIMENSIONS(model, &minDim, &maxDim);
        return (maxDim.z - minDim.z) * 0.5f;
    }

    TpUtil::PositionState TpUtil::saveState()
    {
        int ped = PLAYER::GET_PLAYER_PED(-1);
        int veh = getPlayerVehicle();
        int ent = veh ? veh : ped;
        return {ENTITY::GET_ENTITY_COORDS(ent, TRUE), ENTITY::GET_ENTITY_HEADING(ent), veh};
    }

    TpUtil::TpCoords TpUtil::followRedirects(float x, float y, float z, bool z_is_guessed, bool in_vehicle)
    {
        struct Entry { float bx, by; float vx, vy, vz; float px, py, pz; };
        static constexpr Entry kShops[] = {
            {1697.979126f,  3753.200439f,  1701.130859f,  3750.561523f, 34.367294f,  1697.979126f,  3753.200439f, 34.701405f},
            { 245.271103f,   -45.812595f,   239.232605f,   -43.767391f, 69.180779f,   245.271103f,   -45.812595f, 69.929283f},
            { 844.124817f, -1025.570679f,   843.955627f, -1018.552734f, 27.022779f,   844.124817f, -1025.570679f, 28.183146f},
            {-325.886658f,  6077.020020f,  -321.587433f,  6073.997070f, 30.747419f,  -325.886658f,  6077.020020f, 31.442123f},
            {-664.217773f,  -943.364624f,  -663.850891f,  -949.329773f, 21.019037f,  -664.217773f,  -943.364624f, 21.816385f},
            {-1313.927856f, -390.968933f, -1318.658447f,  -389.834839f, 35.908642f, -1313.927856f,  -390.968933f, 36.684586f},
            {-3165.230713f, 1082.855103f, -3160.738281f,  1081.890137f, 20.234903f, -3165.230713f,  1082.855103f, 20.825485f},
            { 2569.611572f,  302.575989f,  2569.598145f,   310.108673f,107.942314f,  2569.611572f,   302.575989f,108.724243f},
            {-1111.251831f, 2688.476318f, -1111.186401f,  2688.437500f, 18.085384f, -1113.466187f,  2692.016113f, 18.554150f},
            {  811.869873f,-2149.101563f,   812.627136f, -2143.387207f, 28.766768f,   811.869873f, -2149.101563f, 29.619356f},
            {   17.680408f,-1114.287964f,    14.320112f, -1123.469849f, 28.262930f,    17.680408f, -1114.287964f, 29.798706f},
            {  321.6098f,    179.4165f,      320.24875f,   174.75006f, 103.742386f,   322.58475f,    180.0087f,  103.58657f},
            { 1861.6853f,   3750.0798f,    1855.9877f,   3747.6792f,  32.997875f,   1861.5864f,   3750.2366f,   33.031868f},
            { -290.1603f,   6199.0947f,   -287.07327f,   6201.5464f,  31.473038f,  -290.53964f,   6198.3516f,   31.487114f},
            {-1153.9481f,  -1425.0186f,  -1156.1406f,  -1419.0974f,   4.823194f,  -1153.4972f,   -1425.688f,    4.954457f},
            { 1322.4547f,  -1651.1252f,   1319.1198f,   -1646.931f,  52.145573f,   1322.8029f,  -1651.6593f,   52.275066f},
            {-3169.4204f,   1074.7272f,   -3164.598f,    1072.463f,  20.683409f,  -3169.6594f,   1075.0051f,   20.829184f},
            { -821.9946f,   -187.1776f,   -827.5958f,  -189.18474f,  37.61457f,   -820.1551f,  -186.87096f,    37.5689f},
            {  133.5702f,  -1710.918f,    130.37733f,  -1714.8396f,  29.220602f,   135.4413f,  -1709.0634f,   29.29162f},
            {-1287.0822f,  -1116.5576f, -1292.8444f,  -1116.5463f,   6.6531425f,-1285.7589f,  -1116.6903f,    6.9901085f},
            { 1933.1191f,   3726.079f,   1935.5698f,    3721.771f,  32.871326f,  1932.2449f,   3727.5989f,   32.84443f},
            { 1208.3335f,   -470.917f,   1202.8934f,   -470.4119f,  66.244255f,   1210.424f,   -471.9904f,   66.20722f},
            {  -30.7448f,   -148.4921f,  -29.34236f,  -143.49902f,  57.024364f, -32.043205f,   -150.1631f,   57.075897f},
            { -280.8165f,   6231.7705f, -284.91342f,   6235.1514f,  31.483158f,  -278.9422f,   6229.9194f,   31.694422f},
            {  419.531f,    -807.5787f,   414.8278f,  -808.60046f,  29.341763f,  421.97498f,  -807.82825f,   29.491158f},
            { -818.6218f,  -1077.533f,  -816.42365f,  -1081.9867f,  11.1324625f,-818.6504f,  -1077.2657f,   11.339294f},
            {   80.665f,   -1391.6694f,   86.65787f,  -1391.2084f,  29.214544f,  79.77582f,  -1391.3315f,   29.378551f},
            { 1687.8812f,    4820.55f,  1681.2683f,   4821.0103f,  42.058624f,  1690.5027f,   4821.0264f,   42.06316f},
            {-1094.0487f,  2704.1707f, -1092.6343f,   2702.3652f,  19.270864f, -1097.3839f,   2708.6177f,   19.107866f},
            { 1197.9722f,  2704.2205f,  1200.8969f,   2696.5352f,  37.91305f,   1198.7032f,   2705.8027f,   38.22236f},
            {   -0.2361f,  6516.0454f, -3.1778638f,    6519.246f,  31.333563f,  0.24288772f,  6515.4546f,   31.88943f},
            { -715.3598f,   -155.7742f, -719.4518f,  -158.38512f,  36.999577f,  -712.2488f,  -155.63303f,   37.4151f},
            { -158.2199f,   -304.9663f,-151.84828f,   -306.8067f,  38.70402f,  -159.52165f,  -303.4513f,   39.733074f},
            {-1455.0045f,   -233.1862f,-1458.1075f,  -227.35103f,  49.14472f,  -1453.9937f,  -235.01907f,  49.801964f},
            {-1199.8092f,   -776.6886f,-1206.4244f,   -782.0857f,  17.092798f,  -1197.94f,   -775.6148f,   17.324402f},
            {  618.1857f,   2752.5667f,  618.5425f,    2743.446f,  42.01316f,   617.98596f,    2754.42f,   42.088135f},
            {  126.6853f,   -212.5027f, 129.19165f,  -205.22305f,  54.50649f,   125.54211f,  -216.36665f,  54.55783f},
            {-3168.9663f,   1055.2869f,-3165.6106f,   1061.4877f,  20.839468f, -3170.5708f,   1051.7955f,  20.863214f},
        };
        for (const auto& e : kShops)
        {
            if (vDist2D(x, y, e.bx, e.by) < 0.1f)
                return in_vehicle ? TpCoords{e.vx, e.vy, e.vz, true} : TpCoords{e.px, e.py, e.pz, true};
        }
        if (z_is_guessed)
        {
            struct Entry2 { float bx, by, tx, ty, tz; };
            static constexpr Entry2 kInteriors[] = {
                {  987.32f,       79.32f,       988.54395f,    80.86057f,  80.9906f},
                {  927.624329f,   44.851101f,   922.816223f,   47.206078f, 81.106331f},
                {  922.816223f,   47.206078f,   922.816223f,   47.206078f, 81.106331f},
                {  113.240173f, 6624.191406f,   113.240173f, 6624.191406f, 31.264368f},
                {-1147.202759f,-1992.451782f,  -1147.202759f,-1992.451782f, 12.653099f},
                {  724.479248f,-1089.010254f,   724.479248f, -1089.010254f, 21.648642f},
                { -354.587860f, -135.499176f,  -354.587860f,  -135.499176f, 38.479984f},
                { 1174.730347f, 2644.499023f,  1174.730347f,  2644.499023f, 37.240135f},
                { -376.713f,   -1877.56f,        -370.091f,   -1862.77f,   20.5285f},
            };
            for (const auto& e : kInteriors)
            {
                if (vDist2D(x, y, e.bx, e.by) < 0.1f)
                    return {e.tx, e.ty, e.tz, true};
            }
        }
        return {x, y, z, false};
    }

    void TpUtil::onPreTp(const Vector3& pos)
    {
        CommandTeleportParticle::playTpParticle(pos);
    }

    void TpUtil::onPostTp(const Vector3& pos)
    {
        CommandTeleportParticle::playTpParticle(pos);
    }

    void TpUtil::teleportWithRedirects(float x, float y, float z, bool z_is_guessed, std::function<void()>&& callback)
    {
        bool in_vehicle = getPlayerVehicle() != 0;
        auto tp = followRedirects(x, y, z, z_is_guessed, in_vehicle);
        teleport(tp.x, tp.y, tp.z, !tp.z_exact && z_is_guessed, std::move(callback));
    }

    void TpUtil::teleport(float x, float y, float z, bool z_is_guessed, std::function<void()>&& callback)
    {
        if (z_is_guessed)
        {
            FiberPool::queueJob([x, y, z, callback = std::move(callback)]() mutable {
                float gx = x, gy = y, gz = z;
                bool found = false;
                for (int i = 0; i < 60; ++i)
                {
                    STREAMING::REQUEST_COLLISION_AT_COORD(gx, gy, gz);
                    BOOL hit = FALSE;
                    Vector3 endCoords{}, surfaceNormal{};
                    int entityHit = 0;
                    int handle = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
                        gx, gy, 1000.f, gx, gy, -200.f, 1, 0, 7);
                    SHAPETEST::GET_SHAPE_TEST_RESULT(handle, &hit, &endCoords, &surfaceNormal, &entityHit);
                    if (hit)
                    {
                        gz = endCoords.z;
                        found = true;
                        break;
                    }
                    BUILTIN::WAIT(0);
                }
                if (!found)
                    gz = PATH::GET_APPROX_HEIGHT_FOR_POINT(gx, gy);
                float waterZ;
                if (WATER::GET_WATER_HEIGHT(gx, gy, gz, &waterZ))
                    gz = waterZ;
                teleport_exact(gx, gy, gz);
                if (callback) callback();
            });
        }
        else
        {
            teleport_exact(x, y, z);
            if (callback) callback();
        }
    }

    void TpUtil::teleport_exact(float x, float y, float z)
    {
        int ped = PLAYER::GET_PLAYER_PED(-1);
        int veh = getPlayerVehicle();
        int ent = veh ? veh : ped;
        auto prePos = getPlayerPos();
        undo_stack.push(saveState());
        last_tp = Vector3{x, y, z};
        float heading = ENTITY::GET_ENTITY_HEADING(ent);
        float zFinal = z + getEntityHalfHeight(ent);
        onPreTp(prePos);
        ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, x, y, zFinal, TRUE, TRUE, TRUE);
        ENTITY::SET_ENTITY_HEADING(ent, heading);
        onPostTp(Vector3{x, y, zFinal});
    }

    void TpUtil::undo_teleport()
    {
        if (undo_stack.empty())
            return;
        auto state = undo_stack.top();
        undo_stack.pop();
        int ped = PLAYER::GET_PLAYER_PED(-1);
        int ent = (state.veh && ENTITY::DOES_ENTITY_EXIST(state.veh)) ? state.veh : ped;
        auto prePos = getPlayerPos();
        onPreTp(prePos);
        ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, state.coords.x, state.coords.y, state.coords.z, TRUE, TRUE, TRUE);
        ENTITY::SET_ENTITY_HEADING(ent, state.heading);
        onPostTp(state.coords);
    }

    void TpUtil::teleport_to_blip(Blip blip)
    {
        int ent = HUD::GET_BLIP_INFO_ID_ENTITY_INDEX(blip);
        if (ent && ENTITY::DOES_ENTITY_EXIST(ent))
        {
            auto pos = ENTITY::GET_ENTITY_COORDS(ent, TRUE);
            teleport_exact(pos.x, pos.y, pos.z);
        }
        else
        {
            auto coords = HUD::GET_BLIP_COORDS(blip);
            teleportWithRedirects(coords.x, coords.y, coords.z, true);
        }
    }

    void TpUtil::teleport_to_veh(Vehicle veh)
    {
        int numSeats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh);
        int freeSeat = -3;
        for (int i = -1; i < numSeats; ++i)
        {
            if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i, FALSE))
            {
                freeSeat = i;
                break;
            }
        }
        if (freeSeat == -3)
        {
            auto pos = ENTITY::GET_ENTITY_COORDS(veh, TRUE);
            teleport_exact(pos.x, pos.y, pos.z);
        }
        else
        {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            PED::SET_PED_INTO_VEHICLE(ped, veh, freeSeat);
        }
    }
}
