#include "Commands/PlayerCommand.hpp"
#include "Network/ScriptEvent.hpp"

namespace Stand::Features
{
	class NullFunctionKick : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			OnCall(std::vector{player});
		}

		virtual void OnCall(const std::vector<Player>& players) override
		{
			SCRIPT_EVENT_SEND_TO_INTERIOR message;
			message.Interior = 1337;
			for (auto& player : players)
				message.SetPlayer(player.GetId());
			message.Send();
		}
	};

	static NullFunctionKick _NullFunctionKick{"nfkick", "Null Function Kick", "Kicks the player by forcing them to call a null script function"};
}