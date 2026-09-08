#include "Commands/PlayerCommand.hpp"
#include "Weapons/WeaponsHashes.hpp"


namespace Stand::Features
{
	class GivePlayerAllWeapon : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			for (auto weapon : g_WeaponHashes)
				player.GetPed().GiveWeapon(weapon);
		}
	};

	static GivePlayerAllWeapon _GivePlayerAllWeapon{"giveweapons", "Give Weapons", "Gives all weapons to the player"};
}