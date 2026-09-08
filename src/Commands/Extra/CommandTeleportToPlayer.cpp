#include "World/Self.hpp"
#include "Commands/PlayerCommand.hpp"

namespace Stand::Features
{
	class TeleportToPlayer : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			if (auto ped = player.GetPed())
				Self::GetPed().TeleportTo(ped.GetPosition());
		}
	};

	static TeleportToPlayer _TeleportToPlayer{"tptoplayer", "Teleport to Player", "Teleports you to the selected player"};
}