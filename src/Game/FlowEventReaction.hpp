#pragma once

#include "Game/FlowEvent.hpp"
#include "Game/typedecl.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	struct FlowEventReactionData
	{
		floweventreaction_t reactions[1] = { 0 };

		[[nodiscard]] floweventreaction_t& getReactions(playertype_t = 0)
		{
			return reactions[0];
		}

		[[nodiscard]] const floweventreaction_t& getReactions(playertype_t = 0) const
		{
			return reactions[0];
		}
	};

	[[nodiscard]] inline toast_t flow_event_reactions_to_toast_flags(floweventreaction_t& reactions)
	{
		return TOAST_DEFAULT;
	}
}

using Stand::flow_event_reactions_to_toast_flags;
