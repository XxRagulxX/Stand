#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandTrafficLod : public CommandToggle
    {
    public:
        explicit CommandTrafficLod(CommandList* parent)
            : CommandToggle(parent, LIT("Potato Mode"), CMDNAMES("trafficpotato", "trafficlod"))
        {}

        void onEnable(Click& click) final
        {
            CommandTickDispatch::AddCommand(this);
        }

        void onDisable(Click& click) final
        {
            CommandTickDispatch::RemoveCommand(this);
            click.ensureScriptThread([] {
                for (auto veh : Pools::GetVehicles())
                {
                    if (veh)
                        VEHICLE::SET_VEHICLE_LOD_MULTIPLIER(veh.GetHandle(), 1.0f);
                }
            });
        }

        void onTick() final
        {
            for (auto veh : Pools::GetVehicles())
            {
                if (veh)
                    VEHICLE::SET_VEHICLE_LOD_MULTIPLIER(veh.GetHandle(), 0.0f);
            }
        }

        ~CommandTrafficLod() override
        {
            CommandTickDispatch::RemoveCommand(this);
        }
    };
}
