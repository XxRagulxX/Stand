#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Game/Pools.hpp"

#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class CopsDispatch : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(Self::GetPlayer().GetId(), FALSE);
		}

		virtual void OnDisable() override
		{
			PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(Self::GetPlayer().GetId(), TRUE);
		}
	};

	static CopsDispatch _CopsDispatch{"CopsDispatch", "Cops Dispatch", "When you are wanted, no police will be spawned."};

}