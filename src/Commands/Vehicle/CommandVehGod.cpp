#include "Commands/Vehicle/CommandVehGod.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandVehGod::CommandVehGod(CommandList* parent)
        : CommandToggle(parent, LIT("Indestructible"), CMDNAMES("vehgodmode", "indestructible"))
    {
    }

    void CommandVehGod::onChange(Click& click)
    {
        if (m_on) m_lastVeh = 0;
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            if (!m_on) {
                int ped = PLAYER::GET_PLAYER_PED(-1);
                int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) {
                    ENTITY::SET_ENTITY_INVINCIBLE(veh, FALSE, FALSE);
                    VEHICLE::SET_VEHICLE_STRONG(veh, FALSE);
                }
                m_lastVeh = 0;
                return false;
            }
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)
                && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
            {
                if (m_lastVeh != veh) {
                    m_lastVeh = veh;
                    VEHICLE::SET_VEHICLE_FIXED(veh);
                    GRAPHICS::WASH_DECALS_FROM_VEHICLE(veh, 1.0f);
                }
                ENTITY::SET_ENTITY_INVINCIBLE(veh, TRUE, FALSE);
                VEHICLE::SET_VEHICLE_STRONG(veh, TRUE);
                if (ENTITY::IS_ENTITY_IN_WATER(veh))
                    VEHICLE::SET_VEHICLE_ENGINE_ON(veh, TRUE, TRUE, FALSE);
            }
            return true;
        });
    }
}
