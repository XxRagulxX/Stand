#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Vehicle/CommandLastVehicle.hpp"
#include "Commands/Vehicle/CommandFlip.hpp"
#include "Commands/Vehicle/CommandFixVehicle.hpp"
#include "Commands/Vehicle/CommandVehGod.hpp"
#include "Commands/Vehicle/CommandMint.hpp"
#include "Commands/Vehicle/CommandVehInvisibility.hpp"
#include "Commands/Vehicle/CommandVehicleHeadlightsIntensity.hpp"
#include "Commands/Vehicle/CommandNoLockon.hpp"
#include "Commands/Vehicle/CommandDontJackMe.hpp"
#include "Commands/Vehicle/CommandInstaSeat.hpp"
#include "Commands/Vehicle/CommandExitStop.hpp"
#include "Commands/Vehicle/CommandLeaveEngineRunning.hpp"
#include "Commands/Vehicle/SubmarineCar/CommandListSubmarineCar.hpp"
#include "Commands/Vehicle/CommandVehicleSeat.hpp"
#include "Commands/Vehicle/CommandPegasus.hpp"
#include "Commands/Vehicle/CommandToggleEngine.hpp"
#include "Commands/Vehicle/CommandVehicleDestroy.hpp"
#include "Commands/Vehicle/CommandDeleteVehicle.hpp"
#include "Commands/Vehicle/PersonalVehicles/CommandListCurrentPV.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnSettings.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnOnFoot.hpp"
#include "Commands/Vehicle/Spawn/CommandTabSpawnInVehicle.hpp"
#include "Commands/Vehicle/LSC/CommandListLosSantosCustoms.hpp"
#include "Commands/Vehicle/Movement/CommandListMovement.hpp"
#include "Commands/Vehicle/Boost/CommandListRocketBoost.hpp"
#include "Commands/Vehicle/Collisions/CommandListCollisions.hpp"
#include "Commands/Vehicle/Doors/CommandListDoors.hpp"
#include "Commands/Vehicle/ARSpeed/CommandListArSpeed.hpp"
#include "Commands/Vehicle/Countermeasures/CommandListCountermeasures.hpp"
#include "Commands/Vehicle/AutoDrive/CommandListAutoDrive.hpp"
#include "Commands/Vehicle/LightSignals/CommandListLightSignals.hpp"

namespace Stand
{
    class CommandTabVehicle : public CommandList
    {
    public:
        CommandFlip* const flip;
        CommandFixVehicle* const fix;
        CommandVehGod* const vehGod;
        CommandMint* const mint;
        CommandVehInvisibility* const vehInvisibility;
        CommandVehicleHeadlightsIntensity* const headlightsIntensity;
        CommandNoLockon* const noLockon;
        CommandDontJackMe* const dontJackMe;
        CommandInstaSeat* const instaSeat;
        CommandExitStop* const exitStop;
        CommandLeaveEngineRunning* const leaveEngineOn;
        CommandListSubmarineCar* const submarineCar;
        CommandVehicleSeat* const vehicleSeat;
        CommandPegasus* const pegasus;
        CommandToggleEngine* const toggleEngine;
        CommandVehicleDestroy* const vehicleDestroy;
        CommandDeleteVehicle* const deleteVehicle;

        CommandTabSpawnSettings* const spawnSettings;
        CommandTabSpawnOnFoot* const spawnOnFoot;
        CommandTabSpawnInVehicle* const spawnInVehicle;
        CommandListCurrentPV* const currentPV;
        CommandListLastVehicle* const lastVehicle;
        CommandListLosSantosCustoms* const lsc;
        CommandListMovement* const movement;
        CommandListRocketBoost* const rocketBoost;
        CommandListCollisions* const collisions;
        CommandListDoors* const doors;
        CommandListArSpeed* const arSpeed;
        CommandListCountermeasures* const countermeasures;
        CommandListAutoDrive* const autoDrive;
        CommandListLightSignals* const lightSignals;

        explicit CommandTabVehicle()
            : CommandList(nullptr, LIT("Vehicle")),
              flip(createChild<CommandFlip>()),
              fix(createChild<CommandFixVehicle>()),
              vehGod(createChild<CommandVehGod>()),
              mint(createChild<CommandMint>()),
              vehInvisibility(createChild<CommandVehInvisibility>()),
              headlightsIntensity(createChild<CommandVehicleHeadlightsIntensity>()),
              noLockon(createChild<CommandNoLockon>()),
              dontJackMe(createChild<CommandDontJackMe>()),
              instaSeat(createChild<CommandInstaSeat>()),
              exitStop(createChild<CommandExitStop>()),
              leaveEngineOn(createChild<CommandLeaveEngineRunning>()),
              submarineCar(createChild<CommandListSubmarineCar>()),
              vehicleSeat(createChild<CommandVehicleSeat>()),
              pegasus(createChild<CommandPegasus>()),
              toggleEngine(createChild<CommandToggleEngine>()),
              vehicleDestroy(createChild<CommandVehicleDestroy>()),
              deleteVehicle(createChild<CommandDeleteVehicle>()),
              spawnSettings(createChild<CommandTabSpawnSettings>()),
              spawnOnFoot(createChild<CommandTabSpawnOnFoot>()),
              spawnInVehicle(createChild<CommandTabSpawnInVehicle>()),
              currentPV(createChild<CommandListCurrentPV>()),
              lastVehicle(createChild<CommandListLastVehicle>()),
              lsc(createChild<CommandListLosSantosCustoms>()),
              movement(createChild<CommandListMovement>()),
              rocketBoost(createChild<CommandListRocketBoost>()),
              collisions(createChild<CommandListCollisions>()),
              doors(createChild<CommandListDoors>()),
              arSpeed(createChild<CommandListArSpeed>()),
              countermeasures(createChild<CommandListCountermeasures>()),
              autoDrive(createChild<CommandListAutoDrive>()),
              lightSignals(createChild<CommandListLightSignals>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabVehicle& GetCommandTabVehicle();
}
