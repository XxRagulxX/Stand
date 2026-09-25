#pragma once
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    namespace
    {
        class CommandHeadlights : public CommandSlider
        {
        public:
            explicit CommandHeadlights(CommandList* parent)
                : CommandSlider(parent, LIT("Headlights"), CMDNAMES("headlights"), NOLABEL, 0, 12, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandHeadlights() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                if (VEHICLE::IS_TOGGLE_MOD_ON(veh, 22)) {
                    int colour = VEHICLE::GET_VEHICLE_XENON_LIGHT_COLOR_INDEX(veh);
                    setValueIndicator(c, colour == 255 ? 1 : colour + 2);
                } else {
                    setValueIndicator(c, 0);
                }
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    if (v == 0) {
                        VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, false);
                    } else {
                        VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, true);
                        VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(veh, v == 1 ? 255 : v - 2);
                    }
                });
            }
        };
    }
}
