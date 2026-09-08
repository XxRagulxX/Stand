#include "Commands/CommandLegacy.hpp"
#include "Rendering/Notifications.hpp"
#include "Vehicle/PersonalVehicles.hpp"
#include "Core/Pointers.hpp"

namespace Stand::Features
{
	class FixAllVehicles : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			if (!*Pointers.IsSessionStarted)
				return;

			int count = 0;
			for (const auto& it : PersonalVehicles::GetPersonalVehicles())
			{
				const auto& personalVeh = it.second;
				if (personalVeh->Repair())
					count++;
			}

			if (count > 0)
				Notifications::Show("Fix All Vehicles", std::format("{} vehicles fixed.", count), NotificationType::Success);
			else
				Notifications::Show("Fix All Vehicles", "No vehicles to fix.");
		}
	};

	static FixAllVehicles _FixAllVehicles{"fixallvehicles", "Fix All Vehicles", "Fixes all of your destroyed personal vehicles."};
}