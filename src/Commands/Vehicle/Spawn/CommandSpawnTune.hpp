#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
    class CommandSpawnTune : public CommandSlider
    {
    public:
        explicit CommandSpawnTune(CommandList* const parent)
            : CommandSlider(parent, LIT("Tune Spawned Vehicles"), { CMDNAME("spawntune") },
                LIT("Selects the tuning profile for vehicles spawned via Stand."),
                0, 3, 0)
        {
        }

        [[nodiscard]] std::string getValueText() const override
        {
            switch (value)
            {
            case 1:  return "Performance";
            case 2:  return "Performance, Including Spoiler";
            case 3:  return "Fully";
            default: return "Stock";
            }
        }
    };
}
