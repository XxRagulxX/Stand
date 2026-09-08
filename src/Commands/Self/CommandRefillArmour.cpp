#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class RefillArmour : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (!Self::GetPed())
				return;

			Self::GetPed().SetArmour(Self::GetPlayer().GetMaxArmour());
		}
	};

	static RefillArmour _RefillArmour{"refillarmour", "Refill Armour", "Restores your armour to maximum"};
}
