#include "Core/DetourHook.hpp"
#include "Network/NodeHooks.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	bool Hooks::Spoofing::WriteSyncTree(void* tree, int type, int flags, rage::netObject* object, rage::datBitBuffer* buffer, int unk, void* a6, std::uint8_t player_id, void* a8)
	{
		NodeHooks::SetSyncingObject(object);
		NodeHooks::SetSyncingPlayer(Player(player_id));

		auto ret = Hooking::Get<Spoofing::WriteSyncTree>()->Original<decltype(&Spoofing::WriteSyncTree)>()(tree, type, flags, object, buffer, unk, a6, player_id, a8);

		NodeHooks::SetSyncingObject(nullptr);
		NodeHooks::SetSyncingPlayer(nullptr);

		return ret;
	}
}