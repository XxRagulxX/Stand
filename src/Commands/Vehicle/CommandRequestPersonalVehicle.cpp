#include "Commands/CommandLegacy.hpp"
#include "Vehicle/PersonalVehicles.hpp"
#include "Core/Pointers.hpp"

namespace Stand
{
	class RequestPersonalVehicle : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			PersonalVehicles::GetCurrent()->Request();
		}
	};

	static RequestPersonalVehicle _RequestPersonalVehicle{"requestpv", "Request Personal Vehicle", "Requests your current Personal Vehicle."};
}