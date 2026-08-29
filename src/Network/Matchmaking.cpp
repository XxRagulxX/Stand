#include "Network/Matchmaking.hpp"

#include "Rendering/Gui.hpp"
#include "AntiCheat/Hooking.hpp"
#include "Network/is_session.hpp"
#include "Network/MatchmakingConfig.hpp"
#include "Game/pointers.hpp"
#include "Network/rl_matching.hpp"
#include "Network/rlRosGeoLocInfo.hpp"
#include "Network/rlSessionInfo.hpp"

namespace Stand
{
	rage::rlSessionInfo Matchmaking::results[MAX_RESULTS] = {};

	bool Matchmaking::isRequestActive() noexcept
	{
		return status.m_StatusCode == rage::netStatus::PENDING;
	}

	bool Matchmaking::canSendRequest() noexcept
	{
		return false /*!isRequestActive() && !g_gui.isUnloadPending()*/;
	}

	void Matchmaking::sendRequest(const MatchmakingConfig& config)
	{
		/*clear();
		rage::rlMatchingFilter filter{};
		if (config.region != -2)
		{
			if (config.region == -1)
			{
				filter.addCondition(MMATTR_REGION, '==', pointers::rlRos_s_GeoLocInfo->region_code);
			}
			else
			{
				filter.addCondition(MMATTR_REGION, '==', config.region);
			}
		}
		OG(rage_rlScMatchmaking_Find)(0, 1, filter, MAX_RESULTS, results, &num_results, &status);*/
	}

	void Matchmaking::consumeResults(const std::function<void(uhash_t, const rage::rlSessionInfo&)>& consumer)
	{
		for (size_t i = 0; i < num_results; ++i)
		{
			const rage::rlSessionInfo& info = results[i];
			consumer(info.getIdHash(), info);
		}
	}

	SessionAttributes Matchmaking::getSessionAttributes(uhash_t id_hash)
	{
		auto i = session_attributes.find(id_hash);
		if (i != session_attributes.end())
		{
			return i->second;
		}
		return {};
	}

	void Matchmaking::clear() noexcept
	{
		//ZeroMemory(results, sizeof(results));
		session_attributes.clear();
	}
}
