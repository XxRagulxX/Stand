#include "Core/DetourHook.hpp"
#include "Network/NodeHooks.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	bool Hooks::Spoofing::ShouldUseNodeCache(void* node, int flags)
	{
		if (NodeHooks::ShouldSkipNodeCache(reinterpret_cast<CProjectBaseSyncDataNode*>(node)))
			return false;

		return Hooking::Get<Spoofing::ShouldUseNodeCache>()->Original<decltype(&Spoofing::ShouldUseNodeCache)>()(node, flags);
	}
}