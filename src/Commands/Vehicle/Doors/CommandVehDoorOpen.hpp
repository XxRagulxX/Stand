#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    namespace
    {
        class CommandVehDoorOpen : public CommandToggle
        {
            const int door_id;

            [[nodiscard]] std::vector<int> getDoors() const
            {
                if (door_id == -1)
                    return { 0, 1, 2, 3, 4, 5, 6, 7 };
                return { door_id };
            }

        public:
            explicit CommandVehDoorOpen(CommandList* parent, int door_id, Label&& name, std::vector<CommandName> cmdnames = {})
                : CommandToggle(parent, std::move(name), std::move(cmdnames)), door_id(door_id) {}

            void onChange(Click& click) override
            {
                const bool on = m_on;

                if (door_id == -1)
                {
                    for (auto& child : parent->children)
                    {
                        if (auto* sibling = dynamic_cast<CommandVehDoorOpen*>(child.get()))
                        {
                            if (sibling->door_id != -1)
                                sibling->m_on = on;
                        }
                    }
                }

                click.ensureScriptThread([this, on] {
                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                    if (!veh) veh = PED::GET_VEHICLE_PED_IS_IN(ped, true);
                    if (!veh || !ENTITY::DOES_ENTITY_EXIST(veh)) return;
                    for (int door : getDoors())
                    {
                        if (on)
                            VEHICLE::SET_VEHICLE_DOOR_OPEN(veh, door, false, door_id != -1);
                        else
                            VEHICLE::SET_VEHICLE_DOOR_SHUT(veh, door, door_id != -1);
                    }
                });
            }
        };
    }
}
