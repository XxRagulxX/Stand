#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AbstractEntity.hpp"
#include "Core/AllEntitiesEveryTick.hpp"
#include "Network/punishments.hpp"

namespace Stand
{
	class CommandNpcAimPunishment : public CommandToggle
	{
	private:
		const punishment_t mask;

	public:
		explicit CommandNpcAimPunishment(CommandList* const parent, const Punishment& p)
			: CommandToggle(parent, Label(p.name), {}, Label(p.help_text)), mask(p.mask)
		{
		}

		void onEnable(Click& click) final
		{
			AllEntitiesEveryTick::npc_aim_punishments |= mask;
		}

		void onDisable(Click& click) final
		{
			AllEntitiesEveryTick::npc_aim_punishments &= ~mask;
		}
	};
}
