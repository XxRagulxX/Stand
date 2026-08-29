#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/AntiEntitySpam.hpp"

namespace Stand
{
	class CommandAntiEntitySpam : public CommandToggle
	{
	public:
		explicit CommandAntiEntitySpam(CommandList* const parent)
			: CommandToggle(parent, LOC("PTX_ENTSPAM"), { CMDNAME("blockentityspam") }, NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
			AntiEntitySpam::enabled = m_on;
			if (!m_on)
			{
				AntiEntitySpam::onDisable();
			}
		}
	};
}
