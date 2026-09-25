#pragma once
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    namespace
    {
        class CommandNeonLight : public CommandToggle
        {
            const int m_idx;
        public:
            CommandNeonLight(CommandList* parent, Label name, std::vector<CommandName> cmds, int idx)
                : CommandToggle(parent, std::move(name), std::move(cmds))
                , m_idx(idx) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on; int i = m_idx;
                click.ensureScriptThread([on, i] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_NEON_ENABLED(veh, i, on);
                });
            }
        };

        class CommandNeonAll : public CommandToggle
        {
        public:
            explicit CommandNeonAll(CommandList* parent)
                : CommandToggle(parent, LIT("All"), CMDNAMES("vehneonall")) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on;
                click.ensureScriptThread([on] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    for (int i = 0; i < 4; ++i)
                        VEHICLE::SET_VEHICLE_NEON_ENABLED(veh, i, on);
                });
            }
        };
    }
}
