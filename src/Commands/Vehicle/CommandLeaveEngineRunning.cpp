#include "Commands/Vehicle/CommandLeaveEngineRunning.hpp"

#include "Game/ePedConfigFlags.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandLeaveEngineRunning::CommandLeaveEngineRunning(CommandList* parent)
        : CommandToggle(parent, LIT("Leave Engine On When Exiting"), CMDNAMES("leaveengineon"))
    {
    }

    void CommandLeaveEngineRunning::onChange(Click& click)
    {
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            if (!m_on) {
                PED::SET_PED_CONFIG_FLAG(ped, CPED_CONFIG_FLAG_LeaveEngineOnWhenExitingVehicles, FALSE);
                return false;
            }
            PED::SET_PED_CONFIG_FLAG(ped, CPED_CONFIG_FLAG_LeaveEngineOnWhenExitingVehicles, TRUE);
            return true;
        });
    }
}
