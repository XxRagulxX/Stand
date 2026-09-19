#pragma once
#include "Commands/Self/Movement/CommandDrunkMode.hpp"
#include "Commands/Self/Movement/CommandFriction.hpp"
#include "Commands/Self/Movement/CommandGracefulLanding.hpp"
#include "Commands/Self/Movement/CommandNoCollision.hpp"
#include "Commands/Self/Movement/CommandReducedCollision.hpp"
#include "Commands/Self/Movement/CommandSelfFreeze.hpp"
#include "Commands/Self/Movement/CommandSuperJump.hpp"
#include "Commands/Self/Movement/CommandSuperRun.hpp"
#include "Commands/Self/Movement/CommandSwimInAir.hpp"
#include "Commands/Self/Movement/CommandSwimSpeed.hpp"
#include "Commands/Self/Movement/CommandTennisMode.hpp"
#include "Commands/Self/Movement/CommandTotalFreedom.hpp"
#include "Commands/Self/Movement/CommandWalkOnAir.hpp"
#include "Commands/Self/Movement/CommandWalkSpeed.hpp"
#include "Commands/Self/Movement/CommandWalkUnderwater.hpp"
#include "Commands/Self/Movement/CommandWaterWalk.hpp"
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
