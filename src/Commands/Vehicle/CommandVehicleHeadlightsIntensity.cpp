#include "Commands/Vehicle/CommandVehicleHeadlightsIntensity.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandVehicleHeadlightsIntensity::CommandVehicleHeadlightsIntensity(CommandList* parent)
        : CommandSliderFloat(parent, LIT("Headlights Intensity"), CMDNAMES("headlightsintensity"),
              LIT("Only affects your local view."), 0, 1000000, 100, 100)
    {
    }

    void CommandVehicleHeadlightsIntensity::onChange(Click& click, int)
    {
        click.ensureScriptThread([this] {
            if (value == 100) {
                if (m_ticking) {
                    CommandTickDispatch::RemoveCommand(this);
                    m_ticking = false;
                }
            } else if (!m_ticking) {
                CommandTickDispatch::AddCommand(this);
                m_ticking = true;
            }
        });
    }

    void CommandVehicleHeadlightsIntensity::onTick()
    {
        if (value == 100) {
            CommandTickDispatch::RemoveCommand(this);
            m_ticking = false;
            return;
        }
        int ped = PLAYER::GET_PLAYER_PED(-1);
        int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
        if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
            VEHICLE::SET_VEHICLE_LIGHT_MULTIPLIER(veh, value / 100.0f);
    }

    CommandVehicleHeadlightsIntensity::~CommandVehicleHeadlightsIntensity()
    {
        if (m_ticking) CommandTickDispatch::RemoveCommand(this);
    }
}
