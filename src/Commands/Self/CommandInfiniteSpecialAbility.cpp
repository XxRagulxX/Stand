#include "Commands/LoopedCommand.hpp"
#include "World/Self.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class InfiniteSpecialAbility : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			auto id = Self::GetPlayer().GetId();

			if (PLAYER::IS_SPECIAL_ABILITY_UNLOCKED(Self::GetPed().GetModel()) && !PLAYER::IS_SPECIAL_ABILITY_METER_FULL(id, 0))
				PLAYER::SPECIAL_ABILITY_CHARGE_ABSOLUTE(id, 200, true, 0);
		}
	};

	static InfiniteSpecialAbility _InfiniteSpecialAbility{"infspecialability", "Infinite Special Ability", "Ensures that the special ability bar never runs out"};
}