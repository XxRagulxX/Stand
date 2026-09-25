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
        class CommandVehRoofLivery : public CommandSlider
        {
        public:
            explicit CommandVehRoofLivery(CommandList* parent)
                : CommandSlider(parent, LIT("Roof Livery"), CMDNAMES("rooflivery"), NOLABEL, -1, -1, -1)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandVehRoofLivery() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) { setMaxValue(min_value); return; }
                int cnt = VEHICLE::GET_VEHICLE_LIVERY2_COUNT(veh);
                if (cnt <= 0) { setMaxValue(min_value); return; }
                setMaxValue(cnt - 1);
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_LIVERY2(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_LIVERY2(veh, v);
                });
            }
        };
    }
}
