#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Weapons/Weapon.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandFillAmmo : public CommandPhysical
	{
	public:
		explicit CommandFillAmmo(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Fill All Ammo"), CMDNAMES("fillammo"), LIT("Gives maximum ammo for all weapons."))
		{
		}

		void onClick(Click& click) override
		{
			if (auto ped = Stand::Self::GetPed())
			{
				const auto handle = ped.GetHandle();
				for (auto weapon : Weapon::getAllHashes())
				{
					if (WEAPON::HAS_PED_GOT_WEAPON(handle, weapon, FALSE))
						WEAPON::ADD_AMMO_TO_PED(handle, weapon, 9999);
				}
			}
		}
	};
}
