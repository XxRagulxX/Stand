#include "Commands/Stand/CommandDiscord.hpp"

#include "Menu/Click.hpp"

namespace Stand
{
	CommandDiscord::CommandDiscord(CommandList* const parent)
		: CommandToggle(parent, LOC("DISCRP"), { CMDNAME("discordrichpresence"), CMDNAME("discordrpc") })
	{
		instance = this;
	}

	void CommandDiscord::onEnable(Click& click)
	{
		updatePresence();
	}

	void CommandDiscord::onDisable(Click& click)
	{
	}

	void CommandDiscord::update() const
	{
		if (m_on)
		{
			updatePresence();
		}
	}

	void CommandDiscord::updatePresence()
	{
	}
}
