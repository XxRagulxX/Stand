#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Vehicle/Spawn/CommandVehSpawnDrive.hpp"
#include "Commands/Vehicle/Spawn/CommandVehSpawnAir.hpp"
#include "Commands/Vehicle/Spawn/CommandVehSpawnFront.hpp"
#include "Commands/Vehicle/Spawn/CommandVehLikePV.hpp"
#include "Commands/Vehicle/Spawn/CommandVehSpawnDelete.hpp"
#include "Commands/Vehicle/Spawn/CommandVehSpawnVelocity.hpp"

namespace Stand
{
    class CommandTabSpawnInVehicle : public CommandList
    {
    public:
        CommandVehSpawnDrive* const drivespawned;
        CommandVehSpawnAir* const spawnair;
        CommandVehSpawnFront* const spawnfront;
        CommandVehLikePV* const likepv;
        CommandVehSpawnDelete* const deleteprevious;
        CommandVehSpawnVelocity* const keepvelocity;

        explicit CommandTabSpawnInVehicle(CommandList* const parent)
            : CommandList(parent, LIT("In Vehicle Behaviour")),
              drivespawned(createChild<CommandVehSpawnDrive>()),
              spawnair(createChild<CommandVehSpawnAir>()),
              spawnfront(createChild<CommandVehSpawnFront>()),
              likepv(createChild<CommandVehLikePV>()),
              deleteprevious(createChild<CommandVehSpawnDelete>()),
              keepvelocity(createChild<CommandVehSpawnVelocity>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabSpawnInVehicle& GetCommandTabSpawnInVehicle();
}
