#pragma once
#include "Commands/Vehicle/Boost/CommandBoostShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandAutoBoostToggle : public CommandToggle
        {
        public:
            explicit CommandAutoBoostToggle(CommandList* parent)
                : CommandToggle(parent, LIT("Auto Rocket Boost"), CMDNAMES("autoboost"),
                    LIT("Automatically uses the rocket boost when it's charged.")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = BoostVehicle();
                    if (veh) VEHICLE::SET_ROCKET_BOOST_ACTIVE(veh, true);
                    return true;
                });
            }
        };
    }
}
