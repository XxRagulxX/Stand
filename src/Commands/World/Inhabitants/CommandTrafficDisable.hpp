#pragma once
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandTrafficDisable : public CommandListSelect
    {
    public:
        explicit CommandTrafficDisable(CommandList* parent)
            : CommandListSelect(parent, LIT("Disable"), CMDNAMES("notraffic"), NOLABEL, {
                {0, LIT("Disabled")},
                {1, LIT("Enabled")},
                {2, LIT("Enabled (incl. Parked)")}
            }, 0)
        {}

        void onChange(Click& click, long long prev_value) final
        {
            if (value != 0) {
                CommandTickDispatch::AddCommand(this);
            } else {
                CommandTickDispatch::RemoveCommand(this);
                click.ensureScriptThread([] {
                    STREAMING::SET_REDUCE_VEHICLE_MODEL_BUDGET(FALSE);
                    STREAMING::SET_VEHICLE_POPULATION_BUDGET(3);
                    VEHICLE::SET_DISTANT_CARS_ENABLED(TRUE);
                    MISC::POPULATE_NOW();
                });
            }
        }

        void onTick() final
        {
            STREAMING::SET_REDUCE_VEHICLE_MODEL_BUDGET(TRUE);
            STREAMING::SET_VEHICLE_POPULATION_BUDGET(0);
            VEHICLE::SET_DISTANT_CARS_ENABLED(FALSE);
            VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.f);
            VEHICLE::SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.f);
            if (value == 2) {
                VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.f);
            }
        }

        ~CommandTrafficDisable() override
        {
            CommandTickDispatch::RemoveCommand(this);
        }
    };
}
