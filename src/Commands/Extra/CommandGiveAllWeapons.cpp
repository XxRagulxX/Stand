#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"
#include "Weapons/WeaponsHashes.hpp"

namespace Stand::Features
{
	class GiveAllWeapons : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (auto weapon : g_WeaponHashes)
				Self::GetPed().GiveWeapon(weapon);
		}
	};

	static GiveAllWeapons _GiveAllWeapons{"giveallweapons", "Give All Weapons", "Gives you all weapons"};
}