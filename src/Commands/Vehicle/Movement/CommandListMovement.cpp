#include "Commands/Vehicle/Movement/CommandListMovement.hpp"

#include "Commands/Vehicle/Movement/CommandListSuperDrive.hpp"
#include "Commands/Vehicle/Movement/CommandSuperHandbrake.hpp"
#include "Commands/Vehicle/Movement/CommandDriveOnWater.hpp"
#include "Commands/Vehicle/Movement/CommandDriveUnderwater.hpp"
#include "Commands/Vehicle/Movement/CommandSpeedLimit.hpp"
#include "Commands/Vehicle/Movement/CommandListHandlingEditor.hpp"
#include "Commands/Vehicle/Movement/CommandSpecialFlight.hpp"
#include "Commands/Vehicle/Movement/CommandJumpAbility.hpp"
#include "Commands/Vehicle/Movement/CommandGlideAbility.hpp"
#include "Commands/Vehicle/Movement/CommandNoTurbulence.hpp"
#include "Commands/Vehicle/Movement/CommandHeliAutoStabilisation.hpp"
#include "Commands/Vehicle/Movement/CommandListGravityMult.hpp"
#include "Commands/Vehicle/Movement/CommandEnginePowerMult.hpp"
#include "Commands/Vehicle/Movement/CommandListCustomWingsBehaviour.hpp"
#include "Commands/Vehicle/Movement/CommandLessenTyreBurnouts.hpp"
#include "Commands/Vehicle/Movement/CommandSmoothCoasting.hpp"
#include "Commands/Vehicle/Movement/CommandDriftMode.hpp"

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
    CommandListMovement::CommandListMovement(CommandList* parent)
        : CommandList(parent, LIT("Movement"), CMDNAMES_0())
    {
        createChild<CommandListSuperDrive>();
        createChild<CommandSuperHandbrake>();
        createChild<CommandDriveOnWater>();
        createChild<CommandDriveUnderwater>();
        createChild<CommandSpeedLimit>();
        createChild<CommandListHandlingEditor>();
        createChild<CommandSpecialFlight>();
        createChild<CommandJumpAbility>();
        createChild<CommandGlideAbility>();
        createChild<CommandNoTurbulence>();
        createChild<CommandHeliAutoStabilisation>();
        createChild<CommandListGravityMult>();
        createChild<CommandEnginePowerMult>();
        createChild<CommandListCustomWingsBehaviour>();
        createChild<CommandLessenTyreBurnouts>();
        createChild<CommandSmoothCoasting>();
        createChild<CommandDriftMode>();
    }
}
