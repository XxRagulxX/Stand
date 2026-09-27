#include "Commands/Vehicle/CommandToggleEngine.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandToggleEngine::CommandToggleEngine(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Toggle Engine On/Off"), CMDNAMES("toggleengine"),
              LIT("Note that pressing forwards or backwards will cause your character to turn the engine back on in most vehicles."))
    {
    }

    void CommandToggleEngine::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return;
            if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(veh))
                VEHICLE::SET_VEHICLE_ENGINE_ON(veh, FALSE, TRUE, TRUE);
            else
                VEHICLE::SET_VEHICLE_ENGINE_ON(veh, TRUE, TRUE, FALSE);
        });
    }
}
