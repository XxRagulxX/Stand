#include "Commands/Vehicle/Collisions/CommandListCollisions.hpp"

#include "Commands/Vehicle/Collisions/CommandVehRamp.hpp"
#include "Commands/Vehicle/Collisions/CommandVehScoop.hpp"
#include "Commands/Vehicle/Collisions/CommandIncreasedRammingForce.hpp"
#include "Commands/Vehicle/Collisions/CommandGhostVehicles.hpp"
#include "Commands/Vehicle/Collisions/CommandNoPedCollision.hpp"
#include "Commands/Vehicle/Collisions/CommandNoObjCollision.hpp"
#include "Commands/Vehicle/Collisions/CommandNoCollision.hpp"

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    CommandListCollisions::CommandListCollisions(CommandList* parent)
        : CommandList(parent, LIT("Collisions"), CMDNAMES_0())
    {
        createChild<CommandVehRamp>();
        createChild<CommandVehScoop>();
        createChild<CommandIncreasedRammingForce>();
        createChild<CommandGhostVehicles>();
        createChild<CommandNoPedCollision>();
        createChild<CommandNoObjCollision>();
        createChild<CommandVehNoCollision>();
    }
}
