#include "Commands/PlayerCommand.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Scripting/Scripts.hpp"
#include "Network/ScriptEvent.hpp"
#include "Network/GSBD_RandomEvents.hpp"

namespace Stand::Features
{
	class SendPhantomCar : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			Scripts::ForceScriptHost(Scripts::FindScriptThread("freemode"_J));

			if (auto GSBDRandomEvents = GSBD_RandomEvents::Get())
			{
				SCRIPT_EVENT_REQUEST_RANDOM_EVENT data;
				data.FMMCType = 269;
				data.PlayersToSend = 1;
				data.Send();

				Script::current()->yield();

				GSBDRandomEvents->TargetData.PhantomCarTarget = player.GetId();
			}
		}
	};

	static SendPhantomCar _SendPhantomCar{"sendphantomcar", "Send Phantom Car", "Sends the Phantom Car to the specified player."};
}