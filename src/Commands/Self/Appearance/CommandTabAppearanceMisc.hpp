#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Self/Appearance/CommandWalkStyle.hpp"
#include "Commands/Self/Appearance/CommandPreserveWalkStyle.hpp"
#include "Commands/Self/Appearance/CommandNoBlood.hpp"
#include "Commands/Self/Appearance/CommandWetness.hpp"
#include "Commands/Self/Appearance/CommandLockWetness.hpp"
#include "Commands/Self/Appearance/CommandInvisibility.hpp"
#include "Commands/Self/Appearance/CommandDeathEffect.hpp"
#include "Commands/Self/Appearance/CommandRespawnEffect.hpp"
#include "Commands/Self/Appearance/CommandTabAnimations.hpp"
#include "Commands/Self/Appearance/CommandTabPtfxTrails.hpp"

namespace Stand
{
	class CommandTabAppearanceMisc : public CommandList
	{
	public:
		explicit CommandTabAppearanceMisc(CommandList* parent)
			: CommandList(parent, LIT("Character"), CMDNAMES("character"))
		{
			createChild<CommandTabAnimations>();
			createChild<CommandTabPtfxTrails>();
			createChild<CommandWalkStyle>();
			createChild<CommandPreserveWalkStyle>();
			createChild<CommandNoBlood>();
			auto* w = createChild<CommandWetness>();
			createChild<CommandLockWetness>(w);
			createChild<CommandInvisibility>();
			createChild<CommandDeathEffect>();
			createChild<CommandRespawnEffect>();
		}
	};
}
