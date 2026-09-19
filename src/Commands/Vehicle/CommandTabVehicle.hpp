#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Vehicle/CommandSpawnManufacturerNames.hpp"
#include "Commands/Vehicle/CommandSpawnGod.hpp"
#include "Commands/Vehicle/CommandFootSpawnDrive.hpp"
#include "Commands/Vehicle/CommandFootSpawnAir.hpp"
#include "Commands/Vehicle/CommandFootSpawnFront.hpp"
#include "Commands/Vehicle/CommandFootLikePV.hpp"
#include "Commands/Vehicle/CommandFootSpawnDelete.hpp"
#include "Commands/Vehicle/CommandVehSpawnDrive.hpp"
#include "Commands/Vehicle/CommandVehSpawnAir.hpp"
#include "Commands/Vehicle/CommandVehSpawnFront.hpp"
#include "Commands/Vehicle/CommandVehLikePV.hpp"
#include "Commands/Vehicle/CommandVehSpawnDelete.hpp"
#include "Commands/Vehicle/CommandVehSpawnVelocity.hpp"

namespace Stand
{
    class CommandTabVehicle : public CommandList
    {
    public:
        CommandSpawnManufacturerNames* const manufacturerNames;
        CommandSpawnGod* const spawngod;

        CommandFootSpawnDrive* const footDrive;
        CommandFootSpawnAir* const footAir;
        CommandFootSpawnFront* const footFront;
        CommandFootLikePV* const footLikePV;
        CommandFootSpawnDelete* const footDelete;

        CommandVehSpawnDrive* const vehDrive;
        CommandVehSpawnAir* const vehAir;
        CommandVehSpawnFront* const vehFront;
        CommandVehLikePV* const vehLikePV;
        CommandVehSpawnDelete* const vehDelete;
        CommandVehSpawnVelocity* const vehVelocity;

        explicit CommandTabVehicle()
            : CommandList(nullptr, LIT("Vehicle")),
              manufacturerNames(createChild<CommandSpawnManufacturerNames>()),
              spawngod(createChild<CommandSpawnGod>()),
              footDrive(createChild<CommandFootSpawnDrive>()),
              footAir(createChild<CommandFootSpawnAir>()),
              footFront(createChild<CommandFootSpawnFront>()),
              footLikePV(createChild<CommandFootLikePV>()),
              footDelete(createChild<CommandFootSpawnDelete>()),
              vehDrive(createChild<CommandVehSpawnDrive>()),
              vehAir(createChild<CommandVehSpawnAir>()),
              vehFront(createChild<CommandVehSpawnFront>()),
              vehLikePV(createChild<CommandVehLikePV>()),
              vehDelete(createChild<CommandVehSpawnDelete>()),
              vehVelocity(createChild<CommandVehSpawnVelocity>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabVehicle& GetCommandTabVehicle();
}
