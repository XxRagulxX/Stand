#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnManufacturerNames.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnGod.hpp"

namespace Stand
{
    class CommandTabSpawnSettings : public CommandList
    {
    public:
        CommandSpawnManufacturerNames* const manufacturerNames;
        CommandSpawnGod* const spawngod;

        explicit CommandTabSpawnSettings(CommandList* const parent)
            : CommandList(parent, LIT("Spawn Settings")),
              manufacturerNames(createChild<CommandSpawnManufacturerNames>()),
              spawngod(createChild<CommandSpawnGod>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabSpawnSettings& GetCommandTabSpawnSettings();
}
