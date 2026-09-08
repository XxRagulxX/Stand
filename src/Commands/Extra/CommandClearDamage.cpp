#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class ClearDamage : CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			Self::GetPed().ClearDamage();
		}
	};

	static ClearDamage _ClearDamage{"cleardamage", "Clear Damage", "Clears all the dirt and blood on you."};
}