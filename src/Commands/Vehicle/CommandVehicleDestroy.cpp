#include "Commands/Vehicle/CommandVehicleDestroy.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandVehicleDestroy::CommandVehicleDestroy(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Destroy"), CMDNAMES("destroyvehicle"))
    {
    }

    void CommandVehicleDestroy::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
                VEHICLE::EXPLODE_VEHICLE(veh, TRUE, FALSE);
        });
    }
}
