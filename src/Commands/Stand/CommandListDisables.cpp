#include "Commands/Stand/CommandListDisables.hpp"

#include "Commands/Extra/CommandLambdaToggleScriptTick.hpp"
#include "Commands/Widgets/CommandTogglePointer.hpp"
#include "AntiCheat/Hooking.hpp"
#include "Game/natives.hpp"

#include "Commands/Stand/CommandDisableInputs.hpp"
#include "Commands/World/CommandDisablePhone.hpp"
#include "Commands/Self/CommandNoCamShake.hpp"
#include "Commands/Self/CommandNoStuntJumps.hpp"
#include "Commands/Self/CommandNoCinematicCam.hpp"
#include "Commands/Vehicle/CommandDisableVehicleFocus.hpp"
#include "Commands/Self/CommandNoIdleCam.hpp"
#include "Commands/World/CommandDisableRestrictedAreas.hpp"
#include "Commands/World/CommandDisableFlightMusic.hpp"
#include "Commands/World/CommandDisableWantedLevelMusic.hpp"
#include "Commands/Stand/CommandSuppressNumpad7Messages.hpp"

namespace Stand
{
	CommandListDisables::CommandListDisables(CommandList* const parent)
		: CommandList(parent, LOC("DBLES"))
	{
		this->createChild<CommandDisableInputs>();
		this->createChild<CommandDisablePhone>();
		this->createChild<CommandNoCamShake>();
		this->createChild<CommandNoStuntJumps>();
		this->createChild<CommandNoCinematicCam>();
		this->createChild<CommandDisableVehicleFocus>();
		this->createChild<CommandNoIdleCam>();
		this->createChild<CommandLambdaToggleScriptTick>(LOC("DSBLREC"), combineCommandNames(CMDNAMES("no", "block", "disable"), CMDNAME("recordings")), LOC("DSBLREC_H"), [] { RECORDING::CANCEL_REPLAY_RECORDING(); });
		this->createChild<CommandDisableRestrictedAreas>();
		this->createChild<CommandDisableFlightMusic>();
		this->createChild<CommandDisableWantedLevelMusic>();
		this->createChild<CommandSuppressNumpad7Messages>();
	}
}
