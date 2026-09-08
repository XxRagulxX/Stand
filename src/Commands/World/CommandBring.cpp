#include "Commands/PlayerCommand.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class Bring : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			player.TeleportTo(Self::GetPed().GetPosition());
		}
	};

	static Bring _Bring{"bring", "Bring", "Brings the player to you"};
}