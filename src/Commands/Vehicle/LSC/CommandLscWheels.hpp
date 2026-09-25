#pragma once
#include "Commands/Vehicle/LSC/CommandLscShared.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    namespace
    {
        class CommandWheelType : public CommandSlider
        {
            static const char* WheelTypeName(int v) {
                switch (v) {
                case 0:  return "Sport";
                case 1:  return "Muscle";
                case 2:  return "Lowrider";
                case 3:  return "SUV";
                case 4:  return "Offroad";
                case 5:  return "Tuner";
                case 6:  return "Bike";
                case 7:  return "High End";
                case 8:  return "Benny's";
                case 9:  return "Benny's Bespoke";
                case 10: return "Race";
                case 11: return "Street";
                case 12: return "Track";
                default: return "";
                }
            }
        public:
            explicit CommandWheelType(CommandList* parent)
                : CommandSlider(parent, LIT("Wheel Type"), CMDNAMES("wheeltype"), NOLABEL, 0, 12, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandWheelType() override { CommandTickDispatch::RemoveCommand(this); }

            std::string getValueText() const override { return WheelTypeName(value); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, v);
                    VEHICLE::SET_VEHICLE_MOD(veh, Mod::FrontWheels, 0, false);
                });
            }
        };

        class CommandVehDriftTyres : public CommandToggle
        {
        public:
            explicit CommandVehDriftTyres(CommandList* parent)
                : CommandToggle(parent, LIT("Drift Tyres"), CMDNAMES("drifttyres")) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on;
                click.ensureScriptThread([on] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_DRIFT_TYRES(veh, on);
                });
            }
        };

        class CommandVehBulletproofTyres : public CommandToggle
        {
        public:
            explicit CommandVehBulletproofTyres(CommandList* parent)
                : CommandToggle(parent, LIT("Bulletproof Tyres"), CMDNAMES("bulletprooftyres")) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on;
                click.ensureScriptThread([on] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, !on);
                });
            }
        };
    }
}
