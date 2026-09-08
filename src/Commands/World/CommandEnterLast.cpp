#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	class EnterLastVehicle : CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (auto last_veh = Self::GetPed().GetLastVehicle())
				Self::GetPed().SetInVehicle(last_veh);
		}
	};

	static EnterLastVehicle _EnterLastVehicle{"enterlastvehicle", "Enter Last Vehicle", "Enters the last vehicle you were in"};
};