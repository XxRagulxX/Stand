#pragma once

#include "Game/natives.hpp"
#include "Core/AbstractEntity.hpp"
#include "Commands/Widgets/CommandAction.hpp"
#include "Weapons/weapons.hpp"

namespace Stand {
	class CommandRemoveAmmo : public CommandAction {
	public:
		explicit CommandRemoveAmmo(CommandList* const parent)
			: CommandAction(parent, LOC("RMVAMMO"), { CMDNAME("removeammo") }) {}

		void onClick(Click& click) final {
			for(auto const weapon : Weapon::getAllHashes()) {
				if(WEAPON::HAS_PED_GOT_WEAPON(g_player_ped, weapon, FALSE)) {
					WEAPON::SET_PED_AMMO(g_player_ped, weapon, 0, FALSE);
				}
			}
		}
	};
}
