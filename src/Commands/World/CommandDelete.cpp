#include "Commands/CommandLegacy.hpp"
#include "Game/Pools.hpp"
#include "World/Object.hpp"

namespace Stand::Features
{
	class DeleteAllObjects : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (auto obj : Pools::GetObjects())
			{
				if (obj)
					obj.Delete();
			}
		}
	};

	class DeleteAllCameras : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (auto obj : Pools::GetObjects())
			{
				if (obj && obj.As<Object>().IsCamera())
					obj.Delete();
			}
		}
	};

	class DeleteAllPeds : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (auto ped : Pools::GetPeds())
			{
				if (!ped.IsPlayer())
					ped.Delete();
			}
		}
	};

	class DeleteAllVehs : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			for (auto veh : Pools::GetVehicles())
			{
				veh.Delete();
			}
		}
	};

	static DeleteAllCameras _DeleteAllCameras("delcams", "Delete All Cameras", "Deletes all cameras");
	static DeleteAllObjects _DeleteAllObjects{"delobjs", "Delete All Objects", "Deletes all objects in the game world, including mission critical objects"};
	static DeleteAllPeds _DeleteAllPeds{"delpeds", "Delete All Peds", "Deletes all peds in the game world, including mission critical peds"};
	static DeleteAllVehs _DeleteAllVehs{"delvehs", "Delete All Vehicles", "Deletes all vehicles in the game world, including mission critical vehicles"};
}