#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnOnFoot.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnInVehicle.hpp"

namespace Stand
{
    class CommandTabVehicle : public CommandList
    {
    public:
        CommandTabSpawnSettings* const spawnSettings;
        CommandTabSpawnOnFoot* const spawnOnFoot;
        CommandTabSpawnInVehicle* const spawnInVehicle;

        explicit CommandTabVehicle()
            : CommandList(nullptr, LIT("Vehicle")),
              spawnSettings(createChild<CommandTabSpawnSettings>()),
              spawnOnFoot(createChild<CommandTabSpawnOnFoot>()),
              spawnInVehicle(createChild<CommandTabSpawnInVehicle>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabVehicle& GetCommandTabVehicle();
}
