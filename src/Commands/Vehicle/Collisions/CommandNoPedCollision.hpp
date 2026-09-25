#pragma once
#include "Commands/Vehicle/Collisions/CommandCollisionsShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Pools.hpp"

namespace Stand
{
    namespace
    {
        class CommandNoPedCollision : public CommandToggle
        {
        public:
            explicit CommandNoPedCollision(CommandList* parent)
                : CommandToggle(parent, LIT("No Collision With Peds"), CMDNAMES("nopedcollision")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int myVeh = CollVehicle();
                    if (!myVeh) return true;
                    for (auto ped : Pools::GetPeds())
                    {
                        int h = ped.GetHandle();
                        if (h) ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(h, myVeh, TRUE);
                    }
                    return true;
                });
            }
        };
    }
}
