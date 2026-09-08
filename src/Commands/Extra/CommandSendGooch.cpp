#include "Commands/PlayerCommand.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Scripting/Scripts.hpp"
#include "Network/ScriptEvent.hpp"
#include "Network/GSBD_RandomEvents.hpp"

namespace Stand::Features
{
	class SendGooch : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			Scripts::ForceScriptHost(Scripts::FindScriptThread("freemode"_J));

			if (auto GSBDRandomEvents = GSBD_RandomEvents::Get())
			{
				SCRIPT_EVENT_REQUEST_RANDOM_EVENT data;
				data.FMMCType = 310;
				data.PlayersToSend = 1;
				data.Send();

				Script::current()->yield();

				GSBDRandomEvents->TargetData.XMASMuggerTarget = player.GetId();
			}
		}
	};

	static SendGooch _SendGooch{"sendgooch", "Send Gooch", "Sends the Gooch to the specified player."};
}