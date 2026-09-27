#include "Commands/Vehicle/CommandNoLockon.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandNoLockon::CommandNoLockon(CommandList* parent)
        : CommandToggle(parent, LIT("Can't Be Locked On"), CMDNAMES("nolockon", "unlockable"))
    {
    }

    void CommandNoLockon::onChange(Click& click)
    {
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            if (!m_on) {
                int ped = PLAYER::GET_PLAYER_PED(-1);
                int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
                    VEHICLE::SET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON_SYNCED(veh, TRUE, FALSE);
                return false;
            }
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
                VEHICLE::SET_VEHICLE_ALLOW_HOMING_MISSLE_LOCKON_SYNCED(veh, FALSE, FALSE);
            return true;
        });
    }
}
