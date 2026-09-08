#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Scripts.hpp"
#include "Network/GPBD_FM_3.hpp"

namespace Stand::Features
{
	class ForceThunder : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (auto global = GPBD_FM_3::Get(); global && Scripts::SafeToModifyFreemodeBroadcastGlobals())
			{
				global->Entries[Self::GetPlayer().GetId()].BossGoon.ActiveFreemodeEvents[0] |= (1 << 9);
				global->Entries[Self::GetPlayer().GetId()].BossGoon.ActiveFreemodeEvents[0] |= (1 << 10);
				global->Entries[Self::GetPlayer().GetId()].BossGoon.ActiveFreemodeEvents[0] |= (1 << 11);
			}
		}

		virtual void OnDisable() override
		{
			if (auto global = GPBD_FM_3::Get(); global && Scripts::SafeToModifyFreemodeBroadcastGlobals())
			{
				global->Entries[Self::GetPlayer().GetId()].BossGoon.ActiveFreemodeEvents[0] &= ~(1 << 9);
				global->Entries[Self::GetPlayer().GetId()].BossGoon.ActiveFreemodeEvents[0] &= ~(1 << 10);
				global->Entries[Self::GetPlayer().GetId()].BossGoon.ActiveFreemodeEvents[0] &= ~(1 << 11);
			}
		}
	};

	static ForceThunder _ForceThunder{"forcethunder", "Force Thunder", "Forces the weather to thunder for all players in the session"};
}