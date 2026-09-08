#include "Core/Hooks.hpp"
#include "Core/DetourHook.hpp"
#include "Network/CustomMatchmaking.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	bool Matchmaking::MatchmakingUpdate(int profile_index, MatchmakingId* id, int num_slots, int available_slots, rage::rlSessionInfo* info, MatchmakingAttributes* data, rage::rlTaskStatus* status)
	{
		CustomMatchmaking::OnUpdate(num_slots, available_slots, info, data, id);
		return Hooking::Get<Matchmaking::MatchmakingUpdate>()->Original<decltype(&Matchmaking::MatchmakingUpdate)>()(profile_index, id, num_slots, available_slots, info, data, status);
	}
}