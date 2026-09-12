#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Self/CommandWalkStyle.hpp"
#include "Commands/Self/CommandPreserveWalkStyle.hpp"
#include "Commands/Self/CommandNoBlood.hpp"
#include "Commands/Self/CommandWetness.hpp"
#include "Commands/Self/CommandLockWetness.hpp"
#include "Commands/Self/CommandInvisibility.hpp"
#include "Commands/Self/CommandDeathEffect.hpp"
#include "Commands/Self/CommandRespawnEffect.hpp"
#include "Commands/Self/CommandTabAnimations.hpp"

namespace Stand
{
	class CommandTabAppearanceMisc : public CommandList
	{
	public:
		explicit CommandTabAppearanceMisc(CommandList* parent)
			: CommandList(parent, LIT("Character"), CMDNAMES("character"))
		{
			createChild<CommandWalkStyle>();
			createChild<CommandPreserveWalkStyle>();
			createChild<CommandNoBlood>();
			auto* w = createChild<CommandWetness>();
			createChild<CommandLockWetness>(w);
			createChild<CommandInvisibility>();
			createChild<CommandDeathEffect>();
			createChild<CommandRespawnEffect>();
			createChild<CommandTabAnimations>();
		}
	};
}
