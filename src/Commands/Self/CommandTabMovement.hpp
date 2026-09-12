#pragma once
#include "Commands/Self/CommandDrunkMode.hpp"
#include "Commands/Self/CommandFriction.hpp"
#include "Commands/Self/CommandGracefulLanding.hpp"
#include "Commands/Self/CommandNoCollision.hpp"
#include "Commands/Self/CommandReducedCollision.hpp"
#include "Commands/Self/CommandSelfFreeze.hpp"
#include "Commands/Self/CommandSuperJump.hpp"
#include "Commands/Self/CommandSuperRun.hpp"
#include "Commands/Self/CommandSwimInAir.hpp"
#include "Commands/Self/CommandSwimSpeed.hpp"
#include "Commands/Self/CommandTennisMode.hpp"
#include "Commands/Self/CommandTotalFreedom.hpp"
#include "Commands/Self/CommandWalkOnAir.hpp"
#include "Commands/Self/CommandWalkSpeed.hpp"
#include "Commands/Self/CommandWalkUnderwater.hpp"
#include "Commands/Self/CommandWaterWalk.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandTabMovement : public CommandList
	{
	public:
		CommandWalkSpeed* const walkSpeed;
		CommandFriction* const friction;
		CommandGracefulLanding* const gracefulLanding;
		CommandSuperJump* const superJump;
		CommandWalkUnderwater* const walkUnderwater;
		CommandWaterWalk* const waterWalk;
		CommandSwimSpeed* const swimSpeed;
		CommandSuperRun* const superRun;
		CommandTotalFreedom* const totalFreedom;
		CommandWalkOnAir* const walkOnAir;
		CommandSwimInAir* const swimInAir;
		CommandDrunkMode* const drunkMode;
		CommandTennisMode* const tennisMode;
		CommandReducedCollision* const reducedCollision;
		CommandNoCollision* const noCollision;
		CommandSelfFreeze* const selfFreeze;

		explicit CommandTabMovement()
			: CommandList(nullptr, LIT("Movement")),
			  walkSpeed(createChild<CommandWalkSpeed>()),
			  friction(createChild<CommandFriction>()),
			  gracefulLanding(createChild<CommandGracefulLanding>()),
			  superJump(createChild<CommandSuperJump>()),
			  walkUnderwater(createChild<CommandWalkUnderwater>()),
			  waterWalk(createChild<CommandWaterWalk>()),
			  swimSpeed(createChild<CommandSwimSpeed>()),
			  superRun(createChild<CommandSuperRun>()),
			  totalFreedom(createChild<CommandTotalFreedom>()),
			  walkOnAir(createChild<CommandWalkOnAir>()),
			  swimInAir(createChild<CommandSwimInAir>()),
			  drunkMode(createChild<CommandDrunkMode>()),
			  tennisMode(createChild<CommandTennisMode>()),
			  reducedCollision(createChild<CommandReducedCollision>()),
			  noCollision(createChild<CommandNoCollision>()),
			  selfFreeze(createChild<CommandSelfFreeze>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabMovement& GetCommandTabMovement();
}
