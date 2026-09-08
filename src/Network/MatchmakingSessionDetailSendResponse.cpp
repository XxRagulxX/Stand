#include "Core/Hooks.hpp"
#include "Core/DetourHook.hpp"
#include "Network/CustomMatchmaking.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	bool Matchmaking::MatchmakingSessionDetailSendResponse(rage::netConnectionManager* mgr, void* request_frame, rage::rlSessionDetailMsg* msg)
	{
		CustomMatchmaking::OnSendSessionDetailResponse(msg);
		return Hooking::Get<Matchmaking::MatchmakingSessionDetailSendResponse>()->Original<decltype(&Matchmaking::MatchmakingSessionDetailSendResponse)>()(mgr, request_frame, msg);
	}
}