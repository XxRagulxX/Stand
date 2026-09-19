#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandTabSpawnColour : public CommandList
    {
    public:
        CommandToggle* const enabled;

        explicit CommandTabSpawnColour(CommandList* const parent)
            : CommandList(parent, LIT("Colour Spawned Vehicles")),
              enabled(createChild<CommandToggle>(LIT("Colour Spawned Vehicles"), std::vector<CommandName>{ CMDNAME("spawncolour") }, NOLABEL, false))
        {
        }
    };
}
