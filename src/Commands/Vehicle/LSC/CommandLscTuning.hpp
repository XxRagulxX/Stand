#pragma once
#include "Commands/Vehicle/LSC/CommandLscShared.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

#include <cstdlib>

namespace Stand
{
    namespace
    {
        class CommandLscPresetTunings : public CommandPhysical
        {
        public:
            explicit CommandLscPresetTunings(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Preset Tunings"), CMDNAMES_0(), NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class CommandTune : public CommandPhysical
        {
        public:
            explicit CommandTune(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Upgrade"), CMDNAMES("tune", "upgrade", "fullupgrade"), LIT("Fully upgrades your current or last vehicle.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    for (int i = 0; i < 50; ++i) {
                        if (i == 17) continue;
                        int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1;
                        if (mx >= 0) VEHICLE::SET_VEHICLE_MOD(veh, i, mx, false);
                    }
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, true);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 20, true);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, true);
                });
            }
        };

        class CommandPerf : public CommandPhysical
        {
        public:
            explicit CommandPerf(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Performance Upgrade"), CMDNAMES("performance", "perf"), LIT("Upgrades your current or last vehicle's EMS, Brakes, Transmission, Armour and Turbo.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    constexpr int perfMods[] = {
                        Mod::Engine, Mod::Brakes, Mod::Trans, Mod::Suspension, Mod::Armour
                    };
                    for (int t : perfMods) {
                        int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, t) - 1;
                        if (mx >= 0) VEHICLE::SET_VEHICLE_MOD(veh, t, mx, false);
                    }
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, Mod::Turbo, true);
                });
            }
        };

        class CommandPerfWithSpoiler : public CommandPhysical
        {
        public:
            explicit CommandPerfWithSpoiler(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Performance Upgrade, Including Spoiler"), CMDNAMES("perfwithspoiler"), LIT("Upgrades your current or last vehicle's EMS, Brakes, Transmission, Armour, Turbo and best Spoiler.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    constexpr int perfMods[] = {
                        Mod::Spoilers, Mod::Engine, Mod::Brakes,
                        Mod::Trans, Mod::Suspension, Mod::Armour
                    };
                    for (int t : perfMods) {
                        int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, t) - 1;
                        if (mx >= 0) VEHICLE::SET_VEHICLE_MOD(veh, t, mx, false);
                    }
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, Mod::Turbo, true);
                });
            }
        };

        class CommandRandomTune : public CommandPhysical
        {
        public:
            explicit CommandRandomTune(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Random Upgrade"), CMDNAMES("randomtune"), LIT("Applies random mods to your current or last vehicle.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    for (int i = 0; i < 50; ++i) {
                        if (i == 17) continue;
                        int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1;
                        if (mx > 0)
                            VEHICLE::SET_VEHICLE_MOD(veh, i, rand() % (mx + 1), false);
                    }
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, rand() % 2 != 0);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 20, rand() % 2 != 0);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, rand() % 2 != 0);
                });
            }
        };

        class CommandUntune : public CommandPhysical
        {
        public:
            explicit CommandUntune(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Downgrade"), CMDNAMES("untune", "downgrade"), LIT("Resets your current or last vehicle to stock tuning.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    for (int i = 0; i < 50; ++i) {
                        VEHICLE::SET_VEHICLE_MOD(veh, i, -1, false);
                        VEHICLE::TOGGLE_VEHICLE_MOD(veh, i, false);
                    }
                });
            }
        };
    }
}
