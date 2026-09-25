#pragma once
#include "Commands/Vehicle/Collisions/CommandCollisionsShared.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Pools.hpp"

namespace Stand
{
    namespace
    {
        class CommandNoObjCollision : public CommandToggle
        {
        public:
            explicit CommandNoObjCollision(CommandList* parent)
                : CommandToggle(parent, LIT("No Collision With Objects"), CMDNAMES("noobjcollision"),
                    LIT("Note this excludes stationary map objects like trees.")) {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    int myVeh = CollVehicle();
                    if (!m_on)
                    {
                        if (myVeh)
                        {
                            void* vehPtr = Pointers.HandleToPtr(myVeh);
                            for (auto obj : Pools::GetObjects())
                            {
                                int h = obj.GetHandle();
                                if (!h) continue;
                                void* entPtr = Pointers.HandleToPtr(h);
                                if (!entPtr) continue;
                                void* comp = CollFieldAt<void*>(entPtr, 0x50);
                                if (!comp) continue;
                                if (CollFieldAt<void*>(comp, 0x58) == vehPtr)
                                {
                                    CollFieldAt<void*>(comp, 0x58) = nullptr;
                                    CollFieldAt<uint32_t>(comp, 0x68) = 0;
                                }
                            }
                        }
                        return false;
                    }
                    if (!myVeh) return true;
                    for (auto obj : Pools::GetObjects())
                    {
                        int h = obj.GetHandle();
                        if (h)
                            ENTITY::SET_ENTITY_NO_COLLISION_ENTITY(h, myVeh, TRUE);
                    }
                    return true;
                });
            }
        };
    }
}
