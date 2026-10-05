#include "Core/Hooks.hpp"
#include "Core/DetourHook.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	bool Matchmaking::MatchmakingAdvertise(int profile_index, int num_slots, int available_slots, MatchmakingAttributes* data, std::uint64_t session_id, rage::rlSessionInfo* info, MatchmakingId* out_id, rage::rlTaskStatus* status)
	{
		return Hooking::Get<Matchmaking::MatchmakingAdvertise>()->Original<decltype(&Matchmaking::MatchmakingAdvertise)>()(profile_index, num_slots, available_slots, data, session_id, info, out_id, status);
	}
}