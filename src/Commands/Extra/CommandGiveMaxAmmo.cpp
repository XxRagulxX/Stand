#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"
#include "Weapons/Weapon.hpp"

namespace Stand::Features
{
	class GiveMaxAmmo : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (auto weapon : Weapon::getAllHashes())
			{
				Self::GetPed().SetMaxAmmoForWeapon(weapon);
			}
		}
	};

	static GiveMaxAmmo _GiveMaxAmmo{"givemaxammo", "Give Max Ammo", "Refills your ammo to max."};
}