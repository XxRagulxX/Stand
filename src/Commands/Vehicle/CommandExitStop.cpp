#include "Commands/Vehicle/CommandExitStop.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandExitStop::CommandExitStop(CommandList* parent)
        : CommandToggle(parent, LIT("Stop Vehicles When Exiting"), CMDNAMES("exitstop"))
    {
    }

    void CommandExitStop::onChange(Click& click)
    {
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            if (!m_on) return false;
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)
                && TASK::GET_IS_TASK_ACTIVE(ped, 2)
                && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
            {
                ENTITY::SET_ENTITY_VELOCITY(veh, 0.0f, 0.0f, 0.0f);
                VEHICLE::SET_ROCKET_BOOST_ACTIVE(veh, FALSE);
            }
            return true;
        });
    }
}
