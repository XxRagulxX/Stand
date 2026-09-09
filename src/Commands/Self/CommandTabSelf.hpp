#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Self/CommandAutoHeal.hpp"
#include "Commands/Self/CommandFakeWanted.hpp"
#include "Commands/Self/CommandFreezeWanted.hpp"
#include "Commands/Self/CommandInfiniteStamina.hpp"
#include "Commands/Self/CommandParalock.hpp"
#include "Commands/Self/CommandGod.hpp"
#include "Commands/Self/CommandGrace.hpp"
#include "Commands/Self/CommandMaxHealth.hpp"
#include "Commands/Self/CommandSeatGlue.hpp"
#include "Commands/Self/CommandWanted.hpp"

namespace Stand
{
	class CommandTabSelf : public CommandList
	{
	public:
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

		explicit CommandTabSelf() :
		    CommandList(nullptr, LIT("Self")),
		    god(createChild<CommandGod>()),
		    autoHeal(createChild<CommandAutoHeal>()),
		    maxHealth(createChild<CommandMaxHealth>()),
		    grace(createChild<CommandGrace>()),
		    seatGlue(createChild<CommandSeatGlue>()),
		    wanted(createChild<CommandWanted>()),
		    freezeWanted(createChild<CommandFreezeWanted>(wanted)),
		    fakeWanted(createChild<CommandFakeWanted>()),
		    infiniteStamina(createChild<CommandInfiniteStamina>()),
		    paralock(createChild<CommandParalock>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabSelf& GetCommandTabSelf();
}
