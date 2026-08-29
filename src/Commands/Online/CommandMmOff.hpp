#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/Hooking.hpp"

namespace Stand
{
	class CommandMmOff : public CommandToggle
	{
	public:
		explicit CommandMmOff(CommandList* const parent)
			: CommandToggle(parent, LOC("DOFF2"), CMDNAMES("mmdisable"), LOC("MMOFF_H"))
		{
		}

		void onChange(Click& click) final
		{
			g_hooking.disable_matchmaking = m_on;
		}
	};
}
