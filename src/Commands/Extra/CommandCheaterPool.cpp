#include "Commands/LoopedCommand.hpp"
#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Core/Pointers.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Features
{
	class CheaterPool : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		int m_OldRegion;

		virtual void OnEnable() override
		{
			Pointers.SetJoinRequestPoolTypePatch->Apply();
			Pointers.HandleJoinRequestIgnorePoolPatch->Apply();
			m_OldRegion = *Pointers.RegionCode;
		}

		virtual void OnTick() override
		{
			*Pointers.RegionCode = 0; // put everyone in the same region for easier matchmaking
		}

		virtual void OnDisable() override
		{
			Pointers.SetJoinRequestPoolTypePatch->Restore();
			Pointers.HandleJoinRequestIgnorePoolPatch->Restore();
			*Pointers.RegionCode = m_OldRegion;
		}
	};

	static CheaterPool _CheaterPool{"cheaterpool", "Join StandEnhanced-only Sessions", "Matchmaking will put you into sessions with other StandEnhanced users."};
}

namespace Stand::Hooks
{
	int Network::GetPoolType()
	{
		if (Stand::Features::_CheaterPool.GetState())
			return 1;

		return Hooking::Get<Network::GetPoolType>()->Original<decltype(&Network::GetPoolType)>()();
	}
}