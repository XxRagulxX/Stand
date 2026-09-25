#pragma once
#include "Commands/Vehicle/LSC/CommandLscShared.hpp"
#include "Commands/Vehicle/LSC/CommandVehmod.hpp"
#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    namespace
    {
        class CommandListLscPerformance : public CommandList
        {
        public:
            explicit CommandListLscPerformance(CommandList* parent)
                : CommandList(parent, LIT("Performance"), CMDNAMES_0())
            {
                createChild<CommandVehmodInt>(Mod::Armour);
                createChild<CommandVehmodInt>(Mod::Brakes);
                createChild<CommandVehmodInt>(Mod::Engine);
                createChild<CommandVehmodInt>(Mod::Spoilers);
                createChild<CommandVehmodInt>(Mod::Trans);
                createChild<CommandVehmodBool>(Mod::Turbo, CMDNAMES("turbo"));
            }

            bool requiresVehicle() const override { return true; }
            const char* vehicleRequiredMessage() const override
            {
                return s_lsc_in_veh.load(std::memory_order_relaxed) ? nullptr : "Get your ass in a vehicle :/";
            }
        };
    }
}
