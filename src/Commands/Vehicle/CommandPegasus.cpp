#include "Commands/Vehicle/CommandPegasus.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandPegasus::CommandPegasus(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Set As Pegasus Vehicle"), CMDNAMES("pegasus"),
              LIT("Allows you to put certain spawned aircraft in your hangar by making the game think it's a Pegasus vehicle."))
    {
    }

    void CommandPegasus::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            if (!NETWORK::NETWORK_IS_SESSION_STARTED()) return;
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return;
            DECORATOR::DECOR_SET_BOOL(veh, "CreatedByPegasus", TRUE);
            VEHICLE::SET_VEHICLE_HAS_BEEN_OWNED_BY_PLAYER(veh, TRUE);
            int player = PLAYER::PLAYER_ID();
            *ScriptGlobal(GLOBAL_PEGASUS_VEHICLE).as<int*>() = veh;
            *ScriptGlobal(GLOBAL_PLAYER_PEGASUS_VEHICLE).at(player, 1).as<int*>() = veh;
            *ScriptGlobal(GLOBAL_RECLAIM_DISABLED).as<int*>() = TRUE;
        });
    }
}
