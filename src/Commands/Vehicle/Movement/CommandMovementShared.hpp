#pragma once
#include "Core/Pointers.hpp"
#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <cmath>

namespace Stand
{
    namespace
    {
        static int MovVehicle()
        {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) return veh;
            veh = PED::GET_VEHICLE_PED_IS_IN(ped, true);
            return (veh && ENTITY::DOES_ENTITY_EXIST(veh)) ? veh : 0;
        }

        static Vector3 DirFromRot(Vector3 rot)
        {
            float px = rot.x * 3.14159265f / 180.f;
            float pz = rot.z * 3.14159265f / 180.f;
            return { -sinf(pz) * cosf(px), cosf(pz) * cosf(px), sinf(px) };
        }

        static float Lerp(float a, float b, float t) { return a + (b - a) * t; }

        static float DistXY(float x1, float y1, float x2, float y2)
        {
            float dx = x2 - x1, dy = y2 - y1;
            return sqrtf(dx * dx + dy * dy);
        }

        template<typename T>
        static T& FieldAt(void* base, size_t offset)
        {
            return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
        }

        static void* GetVehPtr(int veh)
        {
            return Pointers.HandleToPtr(veh);
        }

        static void* GetHandlingData(void* cveh)
        {
            return FieldAt<void*>(cveh, 0x0960);
        }

        static void* FindSubHandling(void* hdata, int target_type)
        {
            if (!hdata) return nullptr;
            void** arr = FieldAt<void**>(hdata, 0x158);
            uint16_t sz  = FieldAt<uint16_t>(hdata, 0x160);
            for (uint16_t i = 0; i < sz; i++)
            {
                void* sub = arr[i];
                if (!sub) continue;
                using GetTypeFn = int(*)(void*);
                auto** vtbl = *reinterpret_cast<void***>(sub);
                int type = reinterpret_cast<GetTypeFn>(vtbl[3])(sub);
                if (type == target_type)
                    return sub;
            }
            return nullptr;
        }

        static int ButtonFromSlider(int idx)
        {
            switch (idx)
            {
            case 1: return (int)ControllerInputs::INPUT_VEH_BRAKE;
            case 2: return (int)ControllerInputs::INPUT_VEH_HORN;
            case 3: return (int)ControllerInputs::INPUT_VEH_HANDBRAKE;
            default: return (int)ControllerInputs::INPUT_VEH_ACCELERATE;
            }
        }
    }
}
