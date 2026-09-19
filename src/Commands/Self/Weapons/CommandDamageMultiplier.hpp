#pragma once
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandDamageMultiplier : public CommandSliderFloat
	{
	public:
		explicit CommandDamageMultiplier(CommandList* const parent)
			: CommandSliderFloat(parent, LIT("Damage Multiplier"), CMDNAMES("damagemultiplier"),
			                     LIT("Applies a multiplier to the damage dealt by your fists and guns."),
			                     0, 1000000, 100, 10)
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onTick() override
		{
			if (value == 100)
				return;

			const float mult = value / 100.0f;
			const auto  player = Stand::Self::GetPlayer().GetId();
			PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, mult);
			PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(player, mult, TRUE);
			PLAYER::SET_PLAYER_VEHICLE_DAMAGE_MODIFIER(player, mult);
		}

		~CommandDamageMultiplier() override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (value != 100)
			{
				const auto player = Stand::Self::GetPlayer().GetId();
				PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, 1.0f);
				PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(player, 1.0f, TRUE);
				PLAYER::SET_PLAYER_VEHICLE_DAMAGE_MODIFIER(player, 1.0f);
			}
		}
	};
}
