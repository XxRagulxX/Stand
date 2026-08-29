#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Core/AllEntitiesEveryTick.hpp"

namespace Stand
{
	class CommandAimbot : public CommandToggle
	{
	public:
		explicit CommandAimbot(CommandList* const parent)
			: CommandToggle(parent, LOC("AIMBT"), { CMDNAME("aimbot") })
		{
		}

		void onChange(Click& click) final
		{
			AllEntitiesEveryTick::aimbot = m_on;
			if (!m_on)
			{
				ensureScriptThread(click, []
				{
					AllEntitiesEveryTick::aimbotDisable();
				});
			}
		}
	};
}
