#include "Commands/PlayerCommand.hpp"
#include "Network/GSBD_Kicking.hpp"
#include "Scripting/Scripts.hpp"

namespace Stand::Features
{
	class ScriptHostKick : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			if (auto freemode = Scripts::FindScriptThread("freemode"_J); freemode && Scripts::SafeToModifyFreemodeBroadcastGlobals())
			{
				Scripts::ForceScriptHost(freemode);
				GSBD_Kicking::Get()->KickedPlayers[player.GetId()] = true;
			}
		}
	};

	static ScriptHostKick _ScriptHostKick{"shkick", "Script Host Kick", "Kicks the player by faking a vote kick"};
}