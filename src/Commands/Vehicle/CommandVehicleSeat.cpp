#include "Commands/Vehicle/CommandVehicleSeat.hpp"

#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <string>

namespace Stand
{
    CommandVehicleSeat::CommandVehicleSeat(CommandList* parent)
        : CommandSlider(parent, LIT("Set Vehicle Seat"), CMDNAMES("setseat"), NOLABEL, -1, 7, -1)
    {
    }

    std::string CommandVehicleSeat::getValueText() const
    {
        if (value == -1) return "-1 (Driver)";
        return std::to_string(value);
    }

    void CommandVehicleSeat::onChange(Click& click, int)
    {
        click.ensureScriptThread([this] {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
                PED::SET_PED_INTO_VEHICLE(ped, veh, value);
        });
    }
}
