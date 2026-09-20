#include <string>
#include "World/Stats.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Rendering/Notifications.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Globals.hpp"

namespace Stand::Features
{

	class NightClubPopularity : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;
		virtual void OnCall() override
		{
			Stats::SetInt("MPX_CLUB_POPULARITY", 1000); // Nightclub Popularity
		}
	};

	class SupplyCooldownBypass : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			*Globals::FREEMODE_BIKER_SUPPLIES_DELAY.as<int*>() = 0;
			*Globals::FREEMODE_GR_SUPPLIES_DELAY.as<int*>() = 0;
		}

		virtual void OnDisable() override
		{
			*Globals::FREEMODE_BIKER_SUPPLIES_DELAY.as<int*>() = 600;
			*Globals::FREEMODE_GR_SUPPLIES_DELAY.as<int*>() = 600;
		}
	};

	class ResupplyBusiness : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;
		virtual void OnCall() override
		{
			*Globals::HEIST_FLEECA.at(1).at(0).as<int*>() = 1;
			*Globals::HEIST_FLEECA.at(1).at(1).as<int*>() = 1;
			*Globals::HEIST_FLEECA.at(1).at(2).as<int*>() = 1;
			*Globals::HEIST_FLEECA.at(1).at(3).as<int*>() = 1;
			*Globals::HEIST_FLEECA.at(1).at(4).as<int*>() = 1;
			*Globals::HEIST_FLEECA.at(1).at(5).as<int*>() = 1;
			*Globals::HEIST_FLEECA.at(1).at(6).as<int*>() = 1;
		}
	};

	class EnforceEasiestMission : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			auto thread = Scripts::FindScriptThread("gb_biker_contraband_sell"_J);
			if (!thread)
				return;

			// epctLocal_731.f_957 -> mission / vehicle selector
			constexpr int kMissionTypeLocal = 738 + 957;

			auto missionType = ScriptLocal(thread, kMissionTypeLocal).as<int*>();
			if (!missionType)
				return;

			// Force "1 Big Truck" (mission type 0)
			if (*missionType != 0)
			{
				*missionType = 0;
			}
		}
	};

	class HangerResupply : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			FiberPool::queueJob([] {
				if (*Globals::GUN_VAN_VEHICLE.as<int*>() == -1)
				{
					Notifications::Show("Hangar Resupply", "Session not ready. Try again in a moment.", NotificationType::Error);
					return;
				}
				Notifications::Show("Hangar Resupply", "Hangar resupply started.");
				while (true)
				{
					int currentStock = *Globals::GPBD_FM_HANGAR_CRATES.as<int*>();

					if (currentStock >= 50)
					{
						Notifications::Show("Hangar Resupply", "Hangar fully stocked (50 crates).");
						break;
					}
					Stats::SetPackedBool(36828, true);

					Script::current()->yield(30000);
				}
			});
		}
	};

	class WarehouseResupply : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;

		virtual void OnCall() override
		{
			FiberPool::queueJob([] {
				if (*Globals::GUN_VAN_VEHICLE.as<int*>() == -1)
				{
					Notifications::Show("Warehouse Resupply", "Session not ready. Try again in a moment.", NotificationType::Error);
					return;
				}
				Notifications::Show("Warehouse Resupply", "Warehouse resupply started.");
				while (true)
				{
					int currentStock = *Globals::GPBD_FM_WAREHOUSE_STOCK.at(0, 3).as<int*>();

					if (currentStock >= 111)
					{
						Notifications::Show("Warehouse Resupply", "Warehouse fully stocked (111 crates).");
						break;
					}

					Stats::SetPackedBool(32359, true);
					Stats::SetPackedBool(32363, true);

					Script::current()->yield(30000);
				}
			});
		}
	};

	class SalvageyardPopularity : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;
		virtual void OnCall() override
		{
			Stats::SetPackedInt(51051, 100); // Salvage Yard Popularity
		}
	};

	class MoneyfrontHeatremove : public CommandLegacy
	{
		using CommandLegacy::CommandLegacy;
		virtual void OnCall() override
		{
			Stats::SetPackedInt(24924, 0); // Money Fronts Business Heat
			Stats::SetPackedInt(24925, 0);
			Stats::SetPackedInt(24926, 0);
		}
	};



	static NightClubPopularity _NightClubPopularity{"nightclubpopularity", "Max Nightclub Popularity", "Sets Nightclub Popularity to 1000"};
	static SupplyCooldownBypass _SupplyCooldownBypass{"removeresupplycooldown", "Remove ReSupply Cooldown", "Removes Resupply Cooldown"};
	static ResupplyBusiness _resupplybusiness{"resupplybusiness", "Resupply Business", "Resupply all Business"};
	static EnforceEasiestMission _EnforceEasiestMission{"enforceeasiestmission", "Enforce Easiest Mission", "Enforces the easiest mission type for Biker Sell Missions"};
	static HangerResupply _HangerResupply{"hangerresupply", "Resupply Hanger Supplies", "Easyway to resupply hanger supplies"};
	static WarehouseResupply _WarehouseResupply{"warehouseresupply", "Resupply Warehouse Supplies", "Easyway to resupply warehouse"};
	static SalvageyardPopularity _SalvageyardPopularity{"salvageyardpopularity", "Max Salvage Yard Popularity", "set Salvage Yard Popularity to 100"};
	static MoneyfrontHeatremove _MoneyfromHeatremove{"moneyfrontheatremove", "Remove Money Front Heat", "Set Money Front Heat to 0"};
}