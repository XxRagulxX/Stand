#include "Commands/Vehicle/CommandDeleteVehicle.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandDeleteVehicle::CommandDeleteVehicle(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Delete"), CMDNAMES("deletevehicle"),
              LIT("Deletes your current or last vehicle."))
    {
    }

    void CommandDeleteVehicle::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return;
            if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
            {
                NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
                for (int i = 0; i < 30 && !NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh); ++i)
                    BUILTIN::WAIT(0);
            }
            if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
                ENTITY::DELETE_ENTITY(&veh);
        });
    }
}
