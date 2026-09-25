#pragma once
#include "Commands/Vehicle/Collisions/CommandCollisionsShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Pools.hpp"

namespace Stand
{
    namespace
    {
        class CommandGhostVehicles : public CommandToggle
        {
        public:
            explicit CommandGhostVehicles(CommandList* parent)
                : CommandToggle(parent, LIT("No Collision With Other Vehicles"), CMDNAMES("ghostvehicles")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int myVeh = CollVehicle();
                    if (!myVeh) return true;
                    for (auto veh : Pools::GetVehicles())
                    {
                        int h = veh.GetHandle();
                        if (h && h != myVeh)
                            ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(h, myVeh, TRUE);
                    }
                    return true;
                });
            }
        };
    }
}
