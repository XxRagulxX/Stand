#include "Commands/Vehicle/CommandVehInvisibility.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <string>

namespace Stand
{
    CommandVehInvisibility::CommandVehInvisibility(CommandList* parent)
        : CommandSlider(parent, LIT("Invisibility"), CMDNAMES("vehinvisibility"), NOLABEL, 0, 2, 0)
    {
    }

    std::string CommandVehInvisibility::getValueText() const
    {
        static constexpr const char* kNames[] = { "Off", "Locally Visible", "Enabled" };
        return kNames[value];
    }

    void CommandVehInvisibility::onChange(Click& click, int)
    {
        click.ensureScriptThread([this] {
            if (value == 0) {
                int ped = PLAYER::GET_PLAYER_PED(-1);
                int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                if (veh && ENTITY::DOES_ENTITY_EXIST(veh))
                    ENTITY::SET_ENTITY_VISIBLE(veh, TRUE, FALSE);
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

    void CommandVehInvisibility::onTick()
    {
        if (value == 0) {
            CommandTickDispatch::RemoveCommand(this);
            m_ticking = false;
            return;
        }
        int ped = PLAYER::GET_PLAYER_PED(-1);
        int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
        if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return;
        ENTITY::SET_ENTITY_VISIBLE(veh, FALSE, FALSE);
        if (value == 1 && NETWORK::NETWORK_IS_SESSION_STARTED())
            NETWORK::SET_ENTITY_LOCALLY_VISIBLE(veh);
    }

    CommandVehInvisibility::~CommandVehInvisibility()
    {
        if (m_ticking) CommandTickDispatch::RemoveCommand(this);
    }
}
