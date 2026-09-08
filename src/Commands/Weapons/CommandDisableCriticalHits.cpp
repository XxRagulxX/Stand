#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class DisableCriticalHits : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			PED::SET_PED_SUFFERS_CRITICAL_HITS(Self::GetPed().GetHandle(), FALSE);
		}

		virtual void OnDisable() override
		{
			PED::SET_PED_SUFFERS_CRITICAL_HITS(Self::GetPed().GetHandle(), TRUE);
		}
	};

	static DisableCriticalHits _DisableCriticalHits{"disablecriticalhits", "Disable Critical Hits", "Prevent you from getting Critical Hits"};

}