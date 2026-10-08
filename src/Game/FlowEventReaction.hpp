#pragma once

#include "Game/typedecl.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	[[nodiscard]] inline toast_t flow_event_reactions_to_toast_flags(floweventreaction_t& reactions)
	{
		return TOAST_DEFAULT;
	}
}

using Stand::flow_event_reactions_to_toast_flags;
