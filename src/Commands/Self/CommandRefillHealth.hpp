#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Core/AbstractEntity.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	class CommandRefillHealth : public CommandAction
	{
	public:
		explicit CommandRefillHealth(CommandList* const parent)
			: CommandAction(parent, LOC("RFHLT"), { CMDNAME("refillhealth") })
		{
		}

		void onClick(Click& click) final
		{
			g_player_ped.setHealth((float)ENTITY::GET_ENTITY_MAX_HEALTH(g_player_ped));
		}
	};
}
