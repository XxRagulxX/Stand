#include "Commands/PlayerCommand.hpp"

namespace Stand::Features
{
	class DeleteVehicle : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			if (auto ped = player.GetPed())
				Entity::DeleteNetwork(ped.GetVehicleObjectId());
		}
	};

	static DeleteVehicle _DeleteVehicle{"deleteveh", "Delete Vehicle", "Deletes the vehicle the player is in"};
}