#include "Core/DetourHook.hpp"
#include "Network/Players.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	void Info::NetworkPlayerMgrShutdown(CNetworkPlayerMgr* mgr)
	{
		if (!g_Running)
			return Hooking::Get<Info::NetworkPlayerMgrShutdown>()->Original<decltype(&Info::NetworkPlayerMgrShutdown)>()(mgr);

		Players::Shutdown();
		Hooking::Get<Info::NetworkPlayerMgrShutdown>()->Original<decltype(&Info::NetworkPlayerMgrShutdown)>()(mgr);
	}
}