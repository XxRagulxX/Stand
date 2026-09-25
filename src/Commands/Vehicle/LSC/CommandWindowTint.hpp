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
        class CommandWindowTint : public CommandSlider
        {
        public:
            explicit CommandWindowTint(CommandList* parent)
                : CommandSlider(parent, LIT("Window Tint"), CMDNAMES("windowtint"), NOLABEL, 0, 6, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandWindowTint() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_WINDOW_TINT(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, v);
                });
            }
        };
    }
}
