#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Weapons/WeaponsHashes.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandRemoveAmmo : public CommandPhysical
	{
	public:
		explicit CommandRemoveAmmo(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Remove All Ammo"), CMDNAMES("removeammo"), LIT("Removes all ammo from your weapons."))
		{
		}

		void onClick(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
			{
				const auto handle = ped.GetHandle();
				for (auto weapon : g_WeaponHashes)
				{
					if (WEAPON::HAS_PED_GOT_WEAPON(handle, weapon, FALSE))
						WEAPON::SET_PED_AMMO(handle, weapon, 0, FALSE);
				}
			}
		}
	};
}
