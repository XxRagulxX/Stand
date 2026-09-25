#pragma once
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

#include <string>

namespace Stand
{
    namespace
    {
        class CommandVehicleExtra : public CommandToggle
        {
            const int m_extra;
        public:
            CommandVehicleExtra(CommandList* parent, int extra)
                : CommandToggle(parent,
                    Label(std::string("Extra ") + std::to_string(extra), Label::TagLiteral{}),
                    CMDNAMES_0())
                , m_extra(extra)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandVehicleExtra() override
            {
                CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int veh = LscGetVehicle();
                if (!veh || !VEHICLE::DOES_EXTRA_EXIST(veh, m_extra))
                {
                    flags |= CMDFLAG_CONCEALED;
                    return;
                }
                flags &= ~CMDFLAG_CONCEALED;
                m_on = (VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(veh, m_extra) != 0);
            }

            void onChange(Click& click) override
            {
                if (click.isAuto()) return;
                bool on = m_on; int ex = m_extra;
                click.ensureScriptThread([on, ex] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    if (!VEHICLE::DOES_EXTRA_EXIST(veh, ex)) return;
                    VEHICLE::SET_VEHICLE_EXTRA(veh, ex, !on);
                });
            }
        };
    }
}
