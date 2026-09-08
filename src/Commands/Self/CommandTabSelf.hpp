#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Self/CommandAutoHeal.hpp"
#include "Commands/Self/CommandGod.hpp"
#include "Commands/Self/CommandMaxHealth.hpp"

namespace Stand
{
	class CommandTabSelf : public CommandList
	{
	public:
		CommandGod* const god;
		CommandAutoHeal* const autoHeal;
		CommandMaxHealth* const maxHealth;

		explicit CommandTabSelf() :
		    CommandList(nullptr, LIT("Self")),
		    god(createChild<CommandGod>()),
		    autoHeal(createChild<CommandAutoHeal>()),
		    maxHealth(createChild<CommandMaxHealth>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabSelf& GetCommandTabSelf();
}
