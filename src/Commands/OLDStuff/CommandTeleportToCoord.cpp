#include "World/Self.hpp"
#include "Commands/PlayerCommand.hpp"
#include "Commands/CommandPosition3d.hpp"

namespace Stand::Features
{
	static CommandPosition3d _PlayerTeleportCoordinate{"playertpcoord", "Player Teleport Coords", "The coordinates to teleport the player to"};

	class TeleportToCoord : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			player.TeleportTo(_PlayerTeleportCoordinate.GetState());
		}
	};

	static TeleportToCoord _TeleportToCoord{"tpplayertocoord", "Teleport Player To Coord", "Teleports the player to the specified coordinates"};
}