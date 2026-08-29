#pragma once

#include "Commands/Player/CommandPlayerNotify.hpp"

#include "Network/script_events.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlayerNotifyRemoved : public CommandPlayerNotify
	{
	public:
		explicit CommandPlayerNotifyRemoved(CommandList* const parent)
			: CommandPlayerNotify(parent, Util::i32_to_u64_trickery(SE_NOTIFY_REMO), LOC("PLY_NOTIFY_REMO"), { CMDNAME("notifyremoved") })
		{
		}
	};
}
