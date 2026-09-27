#include "Commands/Vehicle/CommandMint.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandMint::CommandMint(CommandList* parent)
        : CommandToggle(parent, LIT("Permanent Mint Condition"), CMDNAMES("mint", "nodecals"))
    {
    }

    void CommandMint::onChange(Click& click)
    {
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            if (!m_on) return false;
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) {
                VEHICLE::SET_VEHICLE_HAS_UNBREAKABLE_LIGHTS(veh, TRUE);
                GRAPHICS::WASH_DECALS_FROM_VEHICLE(veh, 1.0f);
            }
            return true;
        });
    }
}
