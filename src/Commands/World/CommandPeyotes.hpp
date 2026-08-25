#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Config/ManagedTunables.hpp"

namespace Stand
{
	class CommandPeyotes : public CommandToggle
	{
	public:
		explicit CommandPeyotes(CommandList* const parent)
			: CommandToggle(parent, LOC("TNBL_PYTE"), { CMDNAME("peyotes") })
		{
		}

		void onEnable(Click& click) final
		{
			registerAsyncTickEventHandler(click, [this]
			{
				if (!m_on)
				{
					ManagedTunables::peyotes.reset();
					return false;
				}
				ManagedTunables::peyotes.set(true);
				return true;
			});
		}
	};
}
