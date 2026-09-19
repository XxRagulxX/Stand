#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"

namespace Stand
{
    class CommandTabSpawnBlips : public CommandList
    {
    public:
        CommandToggle* const enabled;
        CommandSlider* const colour;
        CommandSliderFloat* const scale;
        CommandToggle* const latestOnly;

        explicit CommandTabSpawnBlips(CommandList* const parent)
            : CommandList(parent, LIT("Blips On Spawned Vehicles")),
              enabled(createChild<CommandToggle>(LIT("Blips On Spawned Vehicles"), CMDNAMES("blipsonspawnedvehicles"), NOLABEL, false)),
              colour(createChild<CommandSlider>(LIT("Colour"), CMDNAMES("spawnedvehiclesblipcolour"), NOLABEL, 0, 85, 3)),
              scale(createChild<CommandSliderFloat>(LIT("Scale"), CMDNAMES("spawnedvehiclesblipscale"), NOLABEL, 20, 200, 100)),
              latestOnly(createChild<CommandToggle>(LIT("Only For Latest Vehicle"), std::vector<CommandName>{}, NOLABEL, false))
        {
        }
    };
}
