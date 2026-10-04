#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class RepairVehicle : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (Self::GetVehicle())
				Self::GetVehicle().Fix();
		}
	};

	static RepairVehicle _RepairVehicle{"repairvehicle", "Repair Vehicle", "Fixes any damage to your current vehicle"};
}