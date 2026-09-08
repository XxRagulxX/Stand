#include "Core/DetourHook.hpp"
#include "Network/NodeHooks.hpp"
#include "Core/Hooks.hpp"
#include "Network/Nodes.hpp"
#include "Core/Hooking.hpp"

#include "Game/datBitBuffer.hpp"

namespace Stand::Hooks
{
	void Spoofing::WriteNodeData(CProjectBaseSyncDataNode* node, rage::netObject* object, rage::datBitBuffer* buffer, void* logger, bool update)
	{
		auto orig_buffer = *buffer;
		bool node_dirty = false;
		Nodes::Init();

		Hooking::Get<Spoofing::WriteNodeData>()->Original<decltype(&Spoofing::WriteNodeData)>()(node, object, buffer, logger, update);

		if (NodeHooks::ModifyNodeData(node, object))
			node_dirty = true;

		if (node_dirty)
		{
			*buffer = orig_buffer;
			Hooking::Get<Spoofing::WriteNodeData>()->Original<decltype(&Spoofing::WriteNodeData)>()(node, object, buffer, logger, false);
		}
	}
}