#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
    class CommandDisablePeds : public CommandToggle
    {
    public:
        explicit CommandDisablePeds(CommandList* parent)
            : CommandToggle(parent, LIT("Disable"), CMDNAMES("nopedestrians", "nopeds"))
        {}

        void onEnable(Click& click) final
        {
            CommandTickDispatch::AddCommand(this);
            click.ensureScriptThread([] {
                for (auto ped : Pools::GetPeds())
                    if (!ped.IsPlayer())
                        ped.Delete();
            });
        }

        void onDisable(Click& click) final
        {
            CommandTickDispatch::RemoveCommand(this);
            click.ensureScriptThread([] {
                STREAMING::SET_REDUCE_PED_MODEL_BUDGET(false);
                STREAMING::SET_PED_POPULATION_BUDGET(3);
                MISC::POPULATE_NOW();
            });
        }

        void onTick() final
        {
            STREAMING::SET_REDUCE_PED_MODEL_BUDGET(true);
            STREAMING::SET_PED_POPULATION_BUDGET(0);
            PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.f);
            PED::SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.f, 0.f);
        }

        ~CommandDisablePeds() override
        {
            CommandTickDispatch::RemoveCommand(this);
        }
    };
}
