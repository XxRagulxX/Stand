#include "Commands/World/CommandDisableDeathbarriers.hpp"

#include "Core/tbDisableDeathbarriers.hpp"

namespace Stand
{
	CommandDisableDeathbarriers::CommandDisableDeathbarriers(CommandList* parent)
		: CommandToggle(parent, LOC("NODTHBAR"), combineCommandNames(CMDNAMES("no", "block", "disable", "bypass"), CMDNAME("deathbarriers")))
	{
	}

	void CommandDisableDeathbarriers::onEnable(Click& click)
	{
		g_tb_disable_deathbarriers.enable();
	}

	void CommandDisableDeathbarriers::onDisable(Click& click)
	{
		g_tb_disable_deathbarriers.disable();
	}
}
