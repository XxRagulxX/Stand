#pragma once

#include "Game/FlowEvent.hpp"
#include "Game/typedecl.hpp"
#include "PlayerType.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	struct FlowEventReactionData
	{
		floweventreaction_t reactions[PlayerType::SIZE] = { 0 };

		[[nodiscard]] floweventreaction_t& getReactions(playertype_t player_type = PlayerType::SELF)
		{
			return reactions[player_type < PlayerType::SIZE ? player_type : 0];
		}

		[[nodiscard]] const floweventreaction_t& getReactions(playertype_t player_type = PlayerType::SELF) const
		{
			return reactions[player_type < PlayerType::SIZE ? player_type : 0];
		}
	};

	[[nodiscard]] inline toast_t flow_event_reactions_to_toast_flags(floweventreaction_t& reactions)
	{
		return TOAST_DEFAULT;
	}
}

using Stand::flow_event_reactions_to_toast_flags;
