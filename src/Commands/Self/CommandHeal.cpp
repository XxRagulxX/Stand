#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class Heal : CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			int maxHealth = Self::GetPed().GetMaxHealth();
			int maxArmour = Self::GetPlayer().GetMaxArmour();
			Self::GetPed().SetHealth(maxHealth);
			Self::GetPed().SetArmour(maxArmour);
		}
	};

	static Heal _Heal{"heal", "Heal", "Restores your health and armour."};
}