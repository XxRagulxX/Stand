#pragma once
#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    namespace
    {
        static int CollVehicle()
        {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) return veh;
            veh = PED::GET_VEHICLE_PED_IS_IN(ped, true);
            return (veh && ENTITY::DOES_ENTITY_EXIST(veh)) ? veh : 0;
        }

        template<typename T>
        static T& CollFieldAt(void* base, size_t offset)
        {
            return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
        }
    }
}
