#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Self/CommandAutoHeal.hpp"
#include "Commands/Self/CommandTabOutfit.hpp"
#include "Commands/Self/CommandFakeWanted.hpp"
#include "Commands/Self/CommandFreezeWanted.hpp"
#include "Commands/Self/CommandInfiniteStamina.hpp"
#include "Commands/Self/CommandParalock.hpp"
#include "Commands/Self/CommandGod.hpp"
#include "Commands/Self/CommandClumsiness.hpp"
#include "Commands/Self/CommandRefillArmour.hpp"
#include "Commands/Self/CommandRefillHealth.hpp"
#include "Commands/Self/CommandRegenerationRate.hpp"
#include "Commands/Self/CommandRespawnDelay.hpp"
#include "Commands/Self/CommandRespawnRecall.hpp"
#include "Commands/Self/CommandGrace.hpp"
#include "Commands/Self/CommandMaxHealth.hpp"
#include "Commands/Self/CommandSeatGlue.hpp"
#include "Commands/Self/CommandSuicide.hpp"
#include "Commands/Self/CommandWanted.hpp"

namespace Stand
{
	class CommandTabSelf : public CommandList
	{
	public:
		CommandTabOutfit* const outfit;
		CommandGod* const god;
		CommandAutoHeal* const autoHeal;
		CommandMaxHealth* const maxHealth;
		CommandGrace* const grace;
		CommandSeatGlue* const seatGlue;
		CommandWanted* const wanted;
		CommandFreezeWanted* const freezeWanted;
		CommandFakeWanted* const fakeWanted;
		CommandInfiniteStamina* const infiniteStamina;
		CommandParalock* const paralock;
		CommandClumsiness* const clumsiness;
		CommandRespawnRecall* const respawnRecall;
		CommandRespawnDelay* const respawnDelay;
		CommandRegenerationRate* const regenerationRate;
		CommandRefillHealth* const refillHealth;
		CommandRefillArmour* const refillArmour;
		CommandSuicide* const suicide;

		explicit CommandTabSelf() :
		    CommandList(nullptr, LIT("Self")),
		    outfit(createChild<CommandTabOutfit>()),
		    god(createChild<CommandGod>()),
		    autoHeal(createChild<CommandAutoHeal>()),
		    maxHealth(createChild<CommandMaxHealth>()),
		    grace(createChild<CommandGrace>()),
		    seatGlue(createChild<CommandSeatGlue>()),
		    wanted(createChild<CommandWanted>()),
		    freezeWanted(createChild<CommandFreezeWanted>(wanted)),
		    fakeWanted(createChild<CommandFakeWanted>()),
		    infiniteStamina(createChild<CommandInfiniteStamina>()),
		    paralock(createChild<CommandParalock>()),
		    clumsiness(createChild<CommandClumsiness>()),
		    respawnRecall(createChild<CommandRespawnRecall>()),
		    respawnDelay(createChild<CommandRespawnDelay>()),
		    regenerationRate(createChild<CommandRegenerationRate>()),
		    refillHealth(createChild<CommandRefillHealth>()),
		    refillArmour(createChild<CommandRefillArmour>()),
		    suicide(createChild<CommandSuicide>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabSelf& GetCommandTabSelf();
}
