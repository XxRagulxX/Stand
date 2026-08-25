#pragma once

#include "Commands/Player/CommandPlayerNotify.hpp"

#include "Network/script_events.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	class CommandPlayerNotifyStolen : public CommandPlayerNotify
	{
	public:
		explicit CommandPlayerNotifyStolen(CommandList* const parent)
			: CommandPlayerNotify(parent, Util::i32_to_u64_trickery(SE_NOTIFY_STOL), LOC("PLY_NOTIFY_STOL"), { CMDNAME("notifystolen") })
		{
		}
	};
}
