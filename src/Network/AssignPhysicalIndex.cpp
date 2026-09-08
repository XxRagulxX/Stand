#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Network/Players.hpp"
#include "Network/CNetGamePlayer.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/FiberPool.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Features
{
	static CommandToggleLegacy _NotifyOnPlayerJoin{
	    "notifyonplayerjoin",
	    "Notify on Player Join",
	    "Notifies you when a player joins above the map",
	    true};
}

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
			Hooking::Get<Info::AssignPhysicalIndex>()->Original<decltype(&Info::AssignPhysicalIndex)>()(mgr,player,index);
			Players::OnPlayerJoin(player);
			if (Features::_NotifyOnPlayerJoin.GetState() && !player->IsLocal())
			{
				std::string name = player->GetName();
				FiberPool::queueJob([name] {
					HUD::SET_TEXT_OUTLINE();
					HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
					HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(std::format("<C>{}</C> joined.", name).data());
					HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, false);
				});
			}
		}
		else
		{
			Players::OnPlayerLeave(player);
			Hooking::Get<Info::AssignPhysicalIndex>()->Original<decltype(&Info::AssignPhysicalIndex)>()(mgr,player,index);
		}
	}
}