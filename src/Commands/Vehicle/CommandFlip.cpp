#include "Commands/Vehicle/CommandFlip.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandFlip::CommandFlip(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Turn Vehicle Upright"), CMDNAMES("flipvehicle"))
    {
    }

    void CommandFlip::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
                ENTITY::SET_ENTITY_ROTATION(veh, 0, 0, CAMERA::GET_GAMEPLAY_CAM_ROT(0).z, 0, TRUE);
        });
    }
}
