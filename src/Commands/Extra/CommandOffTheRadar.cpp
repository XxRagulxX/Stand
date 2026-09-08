#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Core/Pointers.hpp"
#include "Network/GlobalPlayerBD.hpp"

namespace Stand::Features
{
	class OffTheRadar : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (auto gpbd = GlobalPlayerBD::Get(); gpbd && Scripts::SafeToModifyFreemodeBroadcastGlobals())
			{
				*ScriptGlobal(2673276).At(58).As<int*>() = *Pointers.NetworkTime;
				gpbd->Entries[Self::GetPlayer().GetId()].OffRadarActive = true;
			}
		}

		virtual void OnDisable() override
		{
			if (auto gpbd = GlobalPlayerBD::Get(); gpbd && Scripts::SafeToModifyFreemodeBroadcastGlobals())
				gpbd->Entries[Self::GetPlayer().GetId()].OffRadarActive = false;
		}
	};

	static OffTheRadar _OffTheRadar{"otr", "Off The Radar", "Hides you from other players on the radar"};
}