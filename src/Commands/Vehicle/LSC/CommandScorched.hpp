#pragma once
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    namespace
    {
        class CommandScorched : public CommandToggle
        {
        public:
            explicit CommandScorched(CommandList* parent)
                : CommandToggle(parent, LIT("Scorched"), CMDNAMES("scorched", "bbq")) {}

            void onChange(Click& click) override {
                if (!m_on) {
                    click.ensureScriptThread([] {
                        int veh = LscGetVehicle(); if (!veh) return;
                        VEHICLE::SET_VEHICLE_ENVEFF_SCALE(veh, 1.f);
                    });
                    return;
                }
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    VEHICLE::SET_VEHICLE_ENVEFF_SCALE(veh, 0.f);
                    return true;
                });
            }
        };

        class CommandTonk : public CommandToggle
        {
        public:
            explicit CommandTonk(CommandList* parent)
                : CommandToggle(parent, LIT("Remove Turret"), CMDNAMES("tonk", "removeturret")) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on;
                click.ensureScriptThread([on] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_TURRET_HIDDEN(veh, 0, on);
                    VEHICLE::SET_TURRET_HIDDEN(veh, 1, on);
                });
            }
        };
    }
}
