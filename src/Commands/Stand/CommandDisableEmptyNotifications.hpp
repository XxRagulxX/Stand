#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "AntiCheat/Hooking.hpp"

namespace Stand
{
	class CommandDisableEmptyNotifications : public CommandToggle
	{
	public:
		explicit CommandDisableEmptyNotifications(CommandList* const parent)
			: CommandToggle(parent, LOC("NEMTYNTFY"), combineCommandNames(CMDNAMES("no", "disable", "block"), CMDNAMES("emptynotifications", "emptynotifys")), NOLABEL, true)
		{
		}

		void onChange(Click& click) final
		{
			g_hooking.flow_event_reactions[FlowEvent::SE_NOTIFY_EMPTY] = (m_on * REACTION_BLOCK);
		}
	};
}
