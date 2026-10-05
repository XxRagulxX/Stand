#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Network/Players.hpp"
#include "Network/CNetGamePlayer.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	void Info::AssignPhysicalIndex(CNetworkPlayerMgr* mgr, CNetGamePlayer* player, std::uint8_t index)
	{
		if (!g_Running)
		    return Hooking::Get<Info::AssignPhysicalIndex>()->Original<decltype(&Info::AssignPhysicalIndex)>()(mgr, player, index);

		if (index != 255)
		{
			if (player->m_PlayerIndex != 255)
				LOGF(WARNING, "Player {} changed their player index from {} to {}", player->GetName(), player->m_PlayerIndex, index);
			Hooking::Get<Info::AssignPhysicalIndex>()->Original<decltype(&Info::AssignPhysicalIndex)>()(mgr, player, index);
			Players::OnPlayerJoin(player);
		}
		else
		{
			Players::OnPlayerLeave(player);
			Hooking::Get<Info::AssignPhysicalIndex>()->Original<decltype(&Info::AssignPhysicalIndex)>()(mgr, player, index);
		}
	}
}