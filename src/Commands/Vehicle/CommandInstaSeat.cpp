#include "Commands/Vehicle/CommandInstaSeat.hpp"

#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandInstaSeat::CommandInstaSeat(CommandList* parent)
        : CommandToggle(parent, LIT("Instantly Enter & Exit Vehicles"),
              CMDNAMES("instaseat", "instantlyentervehicles"))
    {
    }

    void CommandInstaSeat::onChange(Click& click)
    {
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            if (!m_on) return false;

            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);

            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)
                && PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, (int)ControllerInputs::INPUT_VEH_EXIT))
            {
                if (!m_gaveExitTask) {
                    if (TASK::GET_IS_TASK_ACTIVE(ped, 2))
                        TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
                    TASK::TASK_LEAVE_VEHICLE(ped, veh, 16);
                    m_gaveExitTask = true;
                }
            }
            else
            {
                int tryVeh = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(ped);
                if (tryVeh && !PED::IS_PED_TRYING_TO_ENTER_A_LOCKED_VEHICLE(ped)) {
                    int seat = VEHICLE::GET_VEHICLE_NUMBER_OF_PASSENGERS(tryVeh, 1, 0) == 0
                        ? -1
                        : PED::GET_SEAT_PED_IS_TRYING_TO_ENTER(ped);
                    if (VEHICLE::IS_TURRET_SEAT(tryVeh, seat))
                        seat = -1;
                    if (seat == -1) {
                        int driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(tryVeh, -1, FALSE);
                        if (driver && ENTITY::DOES_ENTITY_EXIST(driver)) {
                            if (PED::IS_PED_A_PLAYER(driver)) {
                                seat = 0;
                            } else if (!TASK::GET_IS_TASK_ACTIVE(driver, 2)) {
                                TASK::TASK_LEAVE_VEHICLE(driver, tryVeh, 16);
                            }
                        }
                    }
                    PED::SET_PED_INTO_VEHICLE(ped, tryVeh, seat);
                } else if (veh) {
                    m_gaveExitTask = false;
                }
            }
            return true;
        });
    }
}
