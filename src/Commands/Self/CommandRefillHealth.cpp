#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Real Stand splits Heal (this project's existing "heal", health +
	// armour together) into two separate one-shot actions - Refill
	// Health and Refill Armour (see CommandRefillArmour.cpp). "heal"
	// itself is untouched (still a real, registered command - just no
	// longer shown on SelfGrid's own Tools row, superseded there by
	// these two matching Stand's own naming).
	class RefillHealth : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (!Self::GetPed())
				return;

			Self::GetPed().SetHealth(Self::GetPed().GetMaxHealth());
		}
	};

	static RefillHealth _RefillHealth{"refillhealth", "Refill Health", "Restores your health to maximum"};
}
