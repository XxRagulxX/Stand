#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AllEntitiesEveryTick.hpp"

namespace Stand
{
	class CommandNpcNeedsToAimAtUser : public CommandToggle
	{
	public:
		explicit CommandNpcNeedsToAimAtUser(CommandList* const parent)
			: CommandToggle(parent, LOC("AIMME"), {}, LOC("AIMMEN"), true)
		{
		}

		void onChange(Click& click) final
		{
			AllEntitiesEveryTick::npc_needs_to_aim_at_user = m_on;
		}
	};
}
