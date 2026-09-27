#include "Commands/Vehicle/CommandFixVehicle.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandFixVehicle::CommandFixVehicle(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Fix Vehicle"), CMDNAMES("fixvehicle", "repairvehicle"))
    {
    }

    void CommandFixVehicle::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) {
                VEHICLE::SET_VEHICLE_FIXED(veh);
                GRAPHICS::WASH_DECALS_FROM_VEHICLE(veh, 1.0f);
            }
        });
    }
}
