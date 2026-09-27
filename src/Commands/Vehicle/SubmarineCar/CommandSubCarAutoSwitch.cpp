#include "Commands/Vehicle/SubmarineCar/CommandSubCarAutoSwitch.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandSubCarAutoSwitch::CommandSubCarAutoSwitch(CommandList* parent)
        : CommandToggle(parent, LIT("Auto Transform Submarine Cars"), CMDNAMES("autosubmarinetransform"))
    {
    }

    void CommandSubCarAutoSwitch::onChange(Click& click)
    {
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            if (!m_on) return false;
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return true;
            bool noAnim = config_noAnim && config_noAnim->m_on;
            if (VEHICLE::IS_VEHICLE_IN_SUBMARINE_MODE(veh)) {
                if (ENTITY::GET_ENTITY_SUBMERGED_LEVEL(veh) <= 0.2f)
                    VEHICLE::TRANSFORM_TO_CAR(veh, noAnim);
            } else {
                if (ENTITY::GET_ENTITY_SUBMERGED_LEVEL(veh) >= 0.8f)
                    VEHICLE::TRANSFORM_TO_SUBMARINE(veh, noAnim);
            }
            return true;
        });
    }
}
