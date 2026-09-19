#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnManufacturerNames.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnGod.hpp"
#include "Commands/Vehicle/Spawn/CommandSpawnTune.hpp"

namespace Stand
{
    class CommandTabSpawnSettings : public CommandList
    {
    public:
        CommandSpawnManufacturerNames* const manufacturerNames;
        CommandSpawnGod* const spawngod;
        CommandSpawnTune* const spawntune;

        explicit CommandTabSpawnSettings(CommandList* const parent)
            : CommandList(parent, LIT("Spawn Settings")),
              manufacturerNames(createChild<CommandSpawnManufacturerNames>()),
              spawngod(createChild<CommandSpawnGod>()),
              spawntune(createChild<CommandSpawnTune>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabSpawnSettings& GetCommandTabSpawnSettings();
}
