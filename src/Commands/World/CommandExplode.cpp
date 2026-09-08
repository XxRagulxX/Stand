#include "Commands/PlayerCommand.hpp"
#include "Game/ExplosionType.hpp"

namespace Stand::Features
{
	class Explode : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			if (auto ped = player.GetPed())
				ped.Explode(ExplosionType::BLIMP, 900.f);
		}
	};

	static Explode _Explode{"explode", "Explode", "Blows the player up"};
}