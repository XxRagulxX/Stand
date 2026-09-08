#include "Network/CustomMatchmaking.hpp"
#include "Util/Joaat.hpp"
#include "Network/MatchmakingId.hpp"
#include "Network/rlSessionDetail.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Core/Hooks.hpp"
#include "Core/DetourHook.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Features
{
	static std::vector<std::pair<int, const char*>> g_RegionCodes = {
	    {0, "CIS"},
	    {1, "South America"},
	    {2, "US East"},
	    {3, "Europe"},
	    {4, "China"},
	    {5, "Australia"},
	    {6, "US West"},
	    {7, "Japan"},
	    {8, "Unknown"},
	};

	static std::vector<std::pair<int, const char*>> g_LanguageTypes = {
	    {0, "English"},
	    {1, "French"},
	    {2, "German"},
	    {3, "Italian"},
	    {4, "Spanish (Spain)"},
	    {5, "Portuguese (Brazil)"},
	    {6, "Polish"},
	    {7, "Russian"},
	    {8, "Korean"},
	    {9, "Chinese (Traditional)"},
	    {10, "Japanese"},
	    {11, "Spanish (Mexico)"},
	    {12, "Chinese (Simplified)"},
	};

	CommandToggleLegacy _SpoofRegionType{
	    "mmspoofregiontype",
	    "Spoof Region Type",
	    "Spoofs the region type of the session"};
	CommandListSelect _RegionType{
	    "mmregiontype",
	    "Region Type",
	    "The region to spoof the session to",
		g_RegionCodes};

	CommandToggleLegacy _SpoofLanguage{
	    "mmspooflanguage",
	    "Spoof Language",
	    "Spoofs the session language"};
	CommandListSelect _Language{
	    "mmlanguage",
	    "Language",
	    "The language to spoof the session to",
	    g_LanguageTypes};

	CommandToggleLegacy _SpoofPlayerCount{
	    "mmspoofplayercount",
	    "Spoof Player Count",
	    "Spoofs the session player count"};
	CommandSliderLegacy _PlayerCount{
	    "mmplayercount",
	    "Player Count",
	    "The player count to spoof the session to",
	    1,
	    32,
	    25};

	CommandToggleLegacy _MultiplexSession{
	    "mmmultiplexsession",
	    "Multiplex Session",
	    "Makes your session much more visible to other players by hosting multiple instances of it. High values may fill your session in seconds. Use at your own risk"};
	CommandSliderLegacy _MultiplexCount{
	    "mmmultiplexsessioncount",
	    "Multiplex Session Count",
	    "The number of sessions to advertise for matchmaking",
	    2,
	    7,
	    5};
}

namespace Stand
{
	static std::uint32_t GetIdHash(MatchmakingId* id)
	{
		return Joaat(id->m_data1);
	}

	static void PatchMatchmakingAttrs(MatchmakingAttributes* attrs)
	{
		if (Features::_SpoofRegionType.GetState())
		{
			attrs->m_ParamValues[4] = Features::_RegionType.GetState();
		}

		if (Features::_SpoofLanguage.GetState())
		{
			attrs->m_ParamValues[3] = Features::_Language.GetState();
		}

		if (Features::_SpoofPlayerCount.GetState())
		{
			attrs->m_ParamValues[2] = Features::_PlayerCount.GetState();
		}
	}

	CustomMatchmaking::CustomMatchmaking()
	{

	}

	bool CustomMatchmaking::OnAdvertiseImpl(int& num_slots, int& available_slots, rage::rlSessionInfo* info, MatchmakingAttributes* attrs, MatchmakingId* id, rage::rlTaskStatus* status)
	{
		PatchMatchmakingAttrs(attrs);

		if (!Features::_MultiplexSession.GetState())
			return true; // let the default game code run

		if (status->m_Status)
			return false;

		status->m_Status = 1; // set task in progress

		auto num_slots_copy = num_slots;
		auto available_slots_copy = available_slots;
		FiberPool::queueJob([this, num_slots_copy, available_slots_copy, info, attrs, id, status] {
			rage::rlTaskStatus first_advert_status;
			if (!Hooking::Get<Hooks::Matchmaking::MatchmakingAdvertise>()->Original<decltype(&Hooks::Matchmaking::MatchmakingAdvertise)>()(0, num_slots_copy, available_slots_copy, attrs, -1, info, id, &first_advert_status))
			{
				LOGF(WARNING, "OnAdvertiseImpl(): MatchmakingAdvertise returned false for the initial advertisement");
				status->m_Status = 2;
				return;
			}

			while (first_advert_status.m_Status == 1)
				Script::current()->yield();

			if (first_advert_status.m_Status == 2)
			{
				LOGF(WARNING, "OnAdvertiseImpl(): MatchmakingAdvertise failed for the initial advertisement with code {}", status->m_ErrorCode);
				status->m_Status = 2;
				return;
			}

			auto id_hash = GetIdHash(id);

			m_MultiplexedSessions.emplace(id_hash, std::vector<MatchmakingId>{});
			
			// create the multiplexed sessions
			for (int i = 0; i < Features::_MultiplexCount.GetState() - 1; i++)
			{
				FiberPool::queueJob([this, num_slots_copy, available_slots_copy, info, attrs, status, id_hash, i] {
					rage::rlTaskStatus additional_advert_status;
					MatchmakingId additional_id;

					if (!Hooking::Get<Hooks::Matchmaking::MatchmakingAdvertise>()->Original<decltype(&Hooks::Matchmaking::MatchmakingAdvertise)>()(0, num_slots_copy, available_slots_copy, attrs, -1, info, &additional_id, &additional_advert_status))
					{
						LOGF(WARNING, "OnAdvertiseImpl(): MatchmakingAdvertise returned false for additional advertisement {}", i);
						return;
					}

					while (additional_advert_status.m_Status == 1)
						Script::current()->yield();

					if (additional_advert_status.m_Status == 2)
					{
						LOGF(WARNING, "OnAdvertiseImpl(): MatchmakingAdvertise failed for the additional advertisement {} with code {}", i, status->m_ErrorCode);
						return;
					}

					if (auto it = m_MultiplexedSessions.find(id_hash); it != m_MultiplexedSessions.end())
						it->second.push_back(additional_id);
				});
			}

			status->m_Status = 3; // signal success to game
		});

		return false; // we're handling the request now
	}

	void CustomMatchmaking::OnUpdateImpl(int& num_slots, int& available_slots, rage::rlSessionInfo* info, MatchmakingAttributes* attrs, MatchmakingId* id)
	{
		PatchMatchmakingAttrs(attrs);
		auto hash = GetIdHash(id);

		// check if the game is trying to update a session we've multiplexed
		if (auto it = m_MultiplexedSessions.find(hash); it != m_MultiplexedSessions.end())
		{
			for (auto& session : it->second)
			{
				auto num_slots_copy = num_slots;
				auto available_slots_copy = available_slots;
				FiberPool::queueJob([session, num_slots_copy, available_slots_copy, info, attrs]() {
					auto session_copy = session; // the compiler doesn't like it if I use session directly
					Hooking::Get<Hooks::Matchmaking::MatchmakingUpdate>()->Original<decltype(&Hooks::Matchmaking::MatchmakingUpdate)>()(0, &session_copy, num_slots_copy, available_slots_copy, info, attrs, nullptr); // life's too short to check the task result
				});
			}
		}
	}

	bool CustomMatchmaking::OnUnadvertiseImpl(MatchmakingId* id)
	{
		auto hash = GetIdHash(id);

		// 1) destroying host session
		if (auto it = m_MultiplexedSessions.find(hash); it != m_MultiplexedSessions.end())
		{
			for (auto& session : it->second)
			{
				FiberPool::queueJob([session]() {
					auto session_copy = session; // the compiler doesn't like it if I use session directly
					Hooking::Get<Hooks::Matchmaking::MatchmakingUnadvertise>()->Original<decltype(&Hooks::Matchmaking::MatchmakingUnadvertise)>()(0, &session_copy, nullptr);
				});
			}

			return true; // we've destroyed the multiplexed sessions, let the game destroy the main session
		}

		// or 2) game tries to unadvertise multiplexed sessions every few seconds
		for (auto& [_, multiplex] : m_MultiplexedSessions)
		{
			for (auto& session : multiplex)
			{
				if (GetIdHash(&session) == hash)
				{
					return false; // don't let the game destroy our sessions
				}
			}
		}

		return true; 
	}

	void CustomMatchmaking::OnSendSessionDetailResponseImpl(rage::rlSessionDetailMsg* message)
	{
		PatchMatchmakingAttrs(&message->m_Detail.m_SessionConfig.m_MatchmakingAttributes);

		if (Features::_SpoofPlayerCount.GetState())
		{
			message->m_Detail.m_PlayerCount = Features::_PlayerCount.GetState();
			message->m_Detail.m_SpectatorCount = 0;
		}
	}
}