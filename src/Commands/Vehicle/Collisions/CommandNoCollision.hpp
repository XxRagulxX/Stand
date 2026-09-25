#pragma once
#include "Commands/Vehicle/Collisions/CommandCollisionsShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandVehNoCollision : public CommandToggle
        {
        public:
            explicit CommandVehNoCollision(CommandList* parent)
                : CommandToggle(parent, LIT("No Collision"), CMDNAMES("vehnocollision")) {}

            void onChange(Click& click) override
            {
                if (!m_on)
                {
                    click.ensureScriptThread([] {
                        int veh = CollVehicle();
                        if (veh) ENTITY::SET_ENTITY_COLLISION(veh, true, false);
                    });
                    return;
                }
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on)
                    {
                        int veh = CollVehicle();
                        if (veh) ENTITY::SET_ENTITY_COLLISION(veh, true, false);
                        return false;
                    }
                    int veh = CollVehicle();
                    if (veh) ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(veh, false, false);
                    return true;
                });
            }
        };
    }
}
