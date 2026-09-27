#include "Commands/Vehicle/CommandDontJackMe.hpp"

#include "Game/ePedConfigFlags.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandDontJackMe::CommandDontJackMe(CommandList* parent)
        : CommandToggle(parent, LIT("Can't Be Dragged Out"), CMDNAMES("nojacking", "nohijacking"))
    {
    }

    void CommandDontJackMe::onChange(Click& click)
    {
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            if (!m_on) {
                PED::SET_PED_CONFIG_FLAG(ped, CPED_CONFIG_FLAG_DontDragMeOutCar, FALSE);
                return false;
            }
            PED::SET_PED_CONFIG_FLAG(ped, CPED_CONFIG_FLAG_DontDragMeOutCar, TRUE);
            return true;
        });
    }
}
