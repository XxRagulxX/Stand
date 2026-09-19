#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Vehicle/Spawn/CommandFootSpawnDrive.hpp"
#include "Commands/Vehicle/Spawn/CommandFootSpawnAir.hpp"
#include "Commands/Vehicle/Spawn/CommandFootSpawnFront.hpp"
#include "Commands/Vehicle/Spawn/CommandFootLikePV.hpp"
#include "Commands/Vehicle/Spawn/CommandFootSpawnDelete.hpp"

namespace Stand
{
    class CommandTabSpawnOnFoot : public CommandList
    {
    public:
        CommandFootSpawnDrive* const drivespawned;
        CommandFootSpawnAir* const spawnair;
        CommandFootSpawnFront* const spawnfront;
        CommandFootLikePV* const likepv;
        CommandFootSpawnDelete* const deleteprevious;

        explicit CommandTabSpawnOnFoot(CommandList* const parent)
            : CommandList(parent, LIT("On Foot Behaviour")),
              drivespawned(createChild<CommandFootSpawnDrive>()),
              spawnair(createChild<CommandFootSpawnAir>()),
              spawnfront(createChild<CommandFootSpawnFront>()),
              likepv(createChild<CommandFootLikePV>()),
              deleteprevious(createChild<CommandFootSpawnDelete>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabSpawnOnFoot& GetCommandTabSpawnOnFoot();
}
