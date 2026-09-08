#include "Commands/CommandLegacy.hpp"
#include "Vehicle/PersonalVehicles.hpp"
#include "Core/Pointers.hpp"

namespace Stand
{
	class DespawnPersonalVehicle : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			PersonalVehicles::GetCurrent()->Despawn();
		}
	};

	static DespawnPersonalVehicle _DespawnPersonalVehicle{"despawnpv", "Despawn Personal Vehicle", "Returns your current Personal Vehicle to storage."};
}