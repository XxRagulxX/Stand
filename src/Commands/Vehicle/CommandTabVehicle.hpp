#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Vehicle/CommandLastVehicle.hpp"
#include "Commands/Vehicle/PersonalVehicles/CommandListCurrentPV.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnOnFoot.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnInVehicle.hpp"
#include "Commands/Vehicle/LSC/CommandListLosSantosCustoms.hpp"
#include "Commands/Vehicle/Movement/CommandListMovement.hpp"

namespace Stand
{
    class CommandTabVehicle : public CommandList
    {
    public:
        CommandTabSpawnSettings* const spawnSettings;
        CommandTabSpawnOnFoot* const spawnOnFoot;
        CommandTabSpawnInVehicle* const spawnInVehicle;
        CommandListCurrentPV* const currentPV;
        CommandListLastVehicle* const lastVehicle;
        CommandListLosSantosCustoms* const lsc;
        CommandListMovement* const movement;

        explicit CommandTabVehicle()
            : CommandList(nullptr, LIT("Vehicle")),
              spawnSettings(createChild<CommandTabSpawnSettings>()),
              spawnOnFoot(createChild<CommandTabSpawnOnFoot>()),
              spawnInVehicle(createChild<CommandTabSpawnInVehicle>()),
              currentPV(createChild<CommandListCurrentPV>()),
              lastVehicle(createChild<CommandListLastVehicle>()),
              lsc(createChild<CommandListLosSantosCustoms>()),
              movement(createChild<CommandListMovement>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabVehicle& GetCommandTabVehicle();
}
