#include "Commands/CommandLegacy.hpp"
#include "World/Self.hpp"
#include "Game/Pools.hpp"

namespace Stand::Features
{
	class BringAllObjs : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			auto pos = Self::GetPed().GetPosition();
			for (auto obj : Pools::GetObjects())
			{
				if (obj)
				{
					obj.ForceControl();
					obj.SetPosition(pos);
				}
			}
		}
	};

	class BringAllPeds : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			auto pos = Self::GetPed().GetPosition();
			for (auto ped : Pools::GetPeds())
			{
				if (!ped.IsPlayer())
				{
					ped.ForceControl();
					ped.SetPosition(pos);
				}
			}
		}
	};

	class BringAllVehs : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			auto pos = Self::GetPed().GetPosition();
			for (auto veh : Pools::GetVehicles())
			{
				veh.ForceControl();
				veh.SetPosition(pos);
			}
		}
	};

	static BringAllObjs _BringAllObjs{"bringobjs", "Bring All Objects", "Teleports all game objects to you"};
	static BringAllPeds _BringAllPeds{"bringpeds", "Bring All Peds", "Teleports all game peds to you"};
	static BringAllVehs _BringAllVehs{"bringvehs", "Bring All Vehicles", "Teleports all game vehicles to you"};
}