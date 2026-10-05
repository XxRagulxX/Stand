#include "Core/Hooks.hpp"
#include "Core/DetourHook.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	bool Matchmaking::MatchmakingUnadvertise(int profile_index, MatchmakingId* id, rage::rlTaskStatus* status)
	{
		return Hooking::Get<Matchmaking::MatchmakingUnadvertise>()->Original<decltype(&Matchmaking::MatchmakingUnadvertise)>()(profile_index, id, status);
	}
}