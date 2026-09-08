#include "Core/DetourHook.hpp"
#include "Network/NodeHooks.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	bool Hooks::Spoofing::IsNodeInScope(void* node, void* a2, std::uint8_t playerId, int flags, unsigned int a5, int a6)
	{
		if (NodeHooks::ShouldForceInScope(reinterpret_cast<CProjectBaseSyncDataNode*>(node)))
			return true;

		return Hooking::Get<Spoofing::IsNodeInScope>()->Original<decltype(&Spoofing::IsNodeInScope)>()(node, a2, playerId, flags, a5, a6);
	}
}