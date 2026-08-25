#include "Commands/Vehicle/CommandTabVehicle.hpp"

#include <map>

#include "Commands/Vehicle/CommandAnyVehRc.hpp"
#include "Commands/Widgets/CommandInputTextLimitChars.hpp"
#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Commands/Extra/CommandLambdaActionScript.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"
#include "Commands/Stand/CommandRainbow.hpp"
#include "Commands/Vehicle/CommandVehicleModelFlag.hpp"
#include "Rendering/create_rainbow.hpp"
#include "Network/OnlineVersion.hpp"
#include "Scripting/Script.hpp"
#include "Vehicle/SpawnedVehicleMgr.hpp"

#include "Vehicle/vehicle_items.hpp"
#include "Commands/Vehicle/CommandVehicle.hpp"
#include "Commands/Vehicle/CommandManufacturerNames.hpp"
#include "Commands/Vehicle/CommandFindVehicle.hpp"
#include "Commands/Vehicle/CommandListOfSpawnedVehicles.hpp"
#include "Commands/Vehicle/CommandSpawnVehicle.hpp"

#include "Commands/Vehicle/CommandGarage.hpp"
#include "Commands/Vehicle/CommandPersonalVehicles.hpp"
#include "Commands/Vehicle/CommandListMyVehicle.hpp"
#include "Commands/Vehicle/CommandRc.hpp"
#include "Commands/Vehicle/CommandVehFollow.hpp"

#include "Commands/Vehicle/CommandCustoms.hpp"

#include "Commands/Vehicle/CommandVehDoorOpen.hpp"
#include "Commands/Vehicle/CommandCountermeasures.hpp"
#include "Commands/Vehicle/CommandAutoDrive.hpp"
#include "Commands/Vehicle/CommandLightSignals.hpp"
#include "Commands/Vehicle/CommandFlip.hpp"
#include "Commands/Vehicle/CommandFixVehicle.hpp"
#include "Commands/Vehicle/CommandVehGod.hpp"
#include "Commands/Vehicle/CommandMint.hpp"
#include "Commands/Vehicle/CommandVehInvisibility.hpp"
#include "Commands/Vehicle/CommandVehicleHeadlightsIntensity.hpp"

#include "Commands/Vehicle/CommandNoTurbulence.hpp"
#include "Commands/Vehicle/CommandDisableHeliAutoStabilisation.hpp"
#include "Commands/Vehicle/CommandVehicleGravityMultiplier.hpp"
#include "Commands/Vehicle/CommandVehicleEnginePowerMultiplier.hpp"
#include "Commands/Vehicle/CommandCustomGlider.hpp"
#include "Commands/Vehicle/CommandLessenTyreBurnouts.hpp"
#include "Commands/Vehicle/CommandSmoothCoasting.hpp"
#include "Commands/Vehicle/CommandDriftMode.hpp"

#include "Commands/Self/CommandArSpeed.hpp"
#include "Commands/Self/CommandSuperDrivePresets.hpp"
#include "Commands/Self/CommandSuperDrive.hpp"
#include "Commands/Self/CommandSuperhandbrake.hpp"
#include "Commands/Vehicle/CommandBoostMod.hpp"
#include "Commands/Vehicle/CommandAutoBoost.hpp"
#include "Commands/World/CommandDriveOnWater.hpp"
#include "Commands/World/CommandWaterwheels.hpp"
#include "Commands/Vehicle/CommandSpeedLimit.hpp"
#include "Commands/Vehicle/CommandListHandling.hpp"
#include "Commands/Self/CommandSpecialFlight.hpp"

#include "Commands/Vehicle/CommandGhostVehicles.hpp"
#include "Commands/Vehicle/CommandVehicleNoPedCollision.hpp"
#include "Commands/Vehicle/CommandVehicleNoObjectCollision.hpp"
#include "Commands/Vehicle/CommandVehicleNoCollision.hpp"

#include "Commands/Vehicle/CommandLockDoors.hpp"

#include "Commands/Weapons/CommandNoLockon.hpp"
#include "Commands/Vehicle/CommandDontJackMe.hpp"

#include "Commands/Vehicle/CommandInstaSeat.hpp"
#include "Commands/Widgets/CommandExitStop.hpp"
#include "Commands/Vehicle/CommandLeaveEngineRunning.hpp"

#include "Commands/Vehicle/CommandSubCarAutoSwitch.hpp"

#include "Commands/Weapons/CommandDrivebyInAny.hpp"
#include "Commands/Weapons/CommandDrivebyWithAny.hpp"
#include "Commands/Vehicle/CommandWheelOnWheels.hpp"
#include "Commands/Weapons/CommandBombTrajectory.hpp"

#include "Commands/Vehicle/CommandFlyThroughWindscreen.hpp"
#include "Commands/Vehicle/CommandLockOnToAllVehicles.hpp"
#include "Commands/Vehicle/CommandVehicleSeat.hpp"

#include "Commands/Vehicle/CommandPegasus.hpp"
#include "Commands/Extra/CommandForgeModel.hpp"

#include "Commands/Vehicle/CommandToggleEngine.hpp"
#include "Commands/Vehicle/CommandVehicleDestroy.hpp"
#include "Commands/Widgets/CommandDelete.hpp"

namespace Stand
{
	CommandTabVehicle::CommandTabVehicle(CommandList* const parent)
		: CommandTab(parent, TAB_VEHICLE, LOC("VEH"), { CMDNAME("tvehicle") })
	{
	}

	void CommandTabVehicle::populate()
	{
		// Spawner
		{
			auto vehicle_spawner = new CommandList(this, LOC("SPWNR"), { CMDNAME("spawner"), CMDNAME("spawnvehicle") });
			this->children.emplace_back(std::unique_ptr<CommandList>(vehicle_spawner));

			auto dlcs = vehicle_spawner->makeChild<CommandList>(LOC("DLCS"));
			std::map<int, std::unique_ptr<CommandList>> dlc_lists{};
			std::unordered_map<const char*, CommandVehicle*> vehicle_commands{};
			for (const VehicleItem* vehicle = &vehicle_items[0]; vehicle != &vehicle_items[COUNT(vehicle_items) ]; vehicle++)
			{
				if (dlc_lists.find(vehicle->dlc.id) == dlc_lists.end())
				{
					dlc_lists.emplace(vehicle->dlc.id, std::make_unique<CommandList>(dlcs.get(), vehicle->dlc.getName()));
				}
				vehicle_commands.emplace(vehicle->name, dlc_lists.at(vehicle->dlc.id)->createChild<CommandVehicle>(vehicle));
			}
			for (auto i = dlc_lists.rbegin(); i != dlc_lists.rend(); i++)
			{
				dlcs->children.emplace_back(std::move(i->second));
			}
			auto classes = vehicle_spawner->makeChild<CommandList>(LOC("SPWNR_CLASSES"));
			std::map<std::string, std::unique_ptr<CommandList>> classes_lists{};
			for (const VehicleItem* vehicle = &vehicle_items[0]; vehicle != &vehicle_items[COUNT(vehicle_items)]; vehicle++)
			{
				auto class_name = Lang::get(vehicle->clazz);
				if (classes_lists.find(class_name) == classes_lists.end())
				{
					classes_lists.emplace(class_name, std::make_unique<CommandList>(classes.get(), LIT(class_name)));
				}
				classes_lists.at(class_name)->createChild<CommandLink>(vehicle_commands.at(vehicle->name));
			}
			for (auto& classes_list : classes_lists)
			{
				std::sort(classes_list.second->children.begin(), classes_list.second->children.end(), [](const std::unique_ptr<Command>& a, const std::unique_ptr<Command>& b)
				{
					return a->as<CommandLink>()->getTarget()->menu_name.literal_str < b->as<CommandLink>()->getTarget()->menu_name.literal_str;
				});
				classes->children.emplace_back(std::move(classes_list.second));
			}

			auto manufacturer_names = vehicle_spawner->makeChild<CommandManufacturerNames>(dlcs.get());
			auto onfoot_list = vehicle_spawner->makeChild<CommandList>(LOC("BHVONFT"));
			auto inveh_list = vehicle_spawner->makeChild<CommandList>(LOC("BHVINVEH"));
			auto spawngod = vehicle_spawner->makeChild<CommandToggle>(LOC("SPWNGOD"), { CMDNAME("spawngod"), CMDNAME("spawnindestructible") });
			auto spawntuned = vehicle_spawner->makeChild<CommandListSelect>(LOC("SPWNTUNE"), { CMDNAME("spawntune") }, LOC("SPWNTUNE_H"), std::vector<CommandListActionItemData>{
				{0, LOC("TUNE_STCK"), CMDNAMES("stock", "off")},
				{ 1, LOC("TUNE_PERF"), CMDNAMES("performance"), LOC("TUNE_PERF_H") },
				{ 2, LOC("TUNE_PERFWSPL"), CMDNAMES("perfwithspoiler") },
				{ 3, LOC("TUNE_FULL"), CMDNAMES("full", "on") },
			}, 1);
			auto spawncolour_list = vehicle_spawner->makeChild<CommandList>(LOC("SPWCLR"));
			auto spawn_plate = vehicle_spawner->makeChild<CommandInputTextLimitChars>(LOC("SPWPLATE"), CMDNAMES_OBF("spawnplate"));

			auto onfoot_spawnin = onfoot_list->createChild<CommandToggle>(LOC("SPWNDRVE"), { CMDNAME("footspawndrive") }, LOC("SPWNDRVE_H"), true);
			auto inveh_spawnin = inveh_list->createChild<CommandToggle>(LOC("SPWNDRVE"), { CMDNAME("vehspawndrive") }, LOC("SPWNDRVE_H"));
			auto onfoot_airinair = onfoot_list->createChild<CommandToggle>(LOC("SPWNAIR"), { CMDNAME("footspawnair") }, NOLABEL, true);
			auto inveh_airinair = inveh_list->createChild<CommandToggle>(LOC("SPWNAIR"), { CMDNAME("vehspawnair") });
			auto onfoot_spawnfront = onfoot_list->createChild<CommandToggle>(LOC("SPWNFRNT"), { CMDNAME("footspawnfront") }, LOC("SPWNFRNT_H"));
			auto inveh_spawnfront = inveh_list->createChild<CommandToggle>(LOC("SPWNFRNT"), { CMDNAME("vehspawnfront") }, LOC("SPWNFRNT_H"), true);
			auto onfoot_likepv = onfoot_list->createChild<CommandToggle>(LOC("SPWLIKEPV"), CMDNAMES_OBF("footlikepv"));
			auto inveh_likepv = inveh_list->createChild<CommandToggle>(LOC("SPWLIKEPV"), CMDNAMES_OBF("vehlikepv"));
			auto onfoot_spawndel = onfoot_list->createChild<CommandToggle>(LOC("SPWNDEL"), { CMDNAME("footspawndelete") });
			auto inveh_spawndel = inveh_list->createChild<CommandToggle>(LOC("SPWNDEL"), { CMDNAME("vehspawndelete") });
			auto inveh_spawnvel = inveh_list->createChild<CommandToggle>(LOC("SPWNVEL"), { CMDNAME("vehspawnvelocity") });
			auto spawncoloured = spawncolour_list->createChild<CommandToggle>(LOC("SPWCLR"));
			auto spawncolourprimary = spawncolour_list->createChild<CommandColour>(LOC("CLRFR"), { CMDNAME("spawnprimarycolour"), CMDNAME("spawnprimarycolor") }, NOLABEL, 255, 0, 255, -1);
			auto spawncoloursecondary = spawncolour_list->createChild<CommandColour>(LOC("CLRSEC"), CMDNAMES("spawnsecondarycolour", "spawnsecondarycolor"), NOLABEL, 255, 0, 255, -1);

			// Main commands
			vehicle_spawner->createChild<CommandFindVehicle>(manufacturer_names.get());
			vehicle_spawner->children.emplace_back(std::move(dlcs));
			vehicle_spawner->children.emplace_back(std::move(classes));
			vehicle_spawner->createChild<CommandListOfSpawnedVehicles>(); // Meta?
			vehicle_spawner->createChild<CommandSpawnVehicle>(spawntuned.get(), onfoot_spawnin, inveh_spawnin, onfoot_airinair, inveh_airinair, onfoot_spawnfront, inveh_spawnfront, onfoot_spawndel, inveh_spawndel, spawngod.get(), inveh_spawnvel, spawncoloured, spawncolourprimary, spawncoloursecondary, onfoot_likepv, inveh_likepv, spawn_plate.get());

			// Settings
			vehicle_spawner->createChild<CommandDivider>(LOC("STTNGS"));
			vehicle_spawner->children.emplace_back(std::move(onfoot_list));
			vehicle_spawner->children.emplace_back(std::move(inveh_list));
			vehicle_spawner->children.emplace_back(std::move(manufacturer_names));
			vehicle_spawner->children.emplace_back(std::move(spawngod));
			vehicle_spawner->children.emplace_back(std::move(spawntuned));
			vehicle_spawner->children.emplace_back(std::move(spawn_plate));
			vehicle_spawner->children.emplace_back(std::move(spawncolour_list));
			{
				auto blip = vehicle_spawner->createChild<CommandList>(LOC("SPWNBLIP"));

				SpawnedVehicleMgr::blips_cmd = blip->createChild<CommandToggle>(LOC("SPWNBLIP"), CMDNAMES_OBF("blipsonspawnedvehicles"));
				SpawnedVehicleMgr::blip_colour = blip->createChild<CommandColour>(LOC("CLR"), CMDNAMES_OBF("spawnedvehiclesblipcolour", "spawnedvehiclesblipcolor"), NOLABEL, 255, 255, 255);
				SpawnedVehicleMgr::blip_scale = blip->createChild<CommandSliderFloat>(LOC("SCLE"), CMDNAMES_OBF("spawnedvehiclesblipscale"), NOLABEL, 20, 200, 75);
				SpawnedVehicleMgr::only_latest = blip->createChild<CommandToggle>(LOC("ONLYLTST"));
				blip->createChild<CommandLambdaActionScript>(LOC("CLRBLIP"), CMDNAMES_OBF("clearspawnedblips"), NOLABEL, [](const Click& click)
				{
					SpawnedVehicleMgr::clearBlips();
				});
			}
		}
		this->createChildWithBuilder<CommandGarage>();
		this->createChildWithBuilder<CommandPersonalVehicles>();
		{
			auto cur_pv = this->createChild<CommandListMyVehicle>(true);
			cur_pv->createChild<CommandLambdaAction>(LOC("GTARC"), {}, NOLABEL, [](Click& click)
			{
				if (!click.inOnline())
				{
					return;
				}
				auto veh = AbstractEntity::getPersonalVehicle();
				if (!veh.isValid())
				{
					return click.setResponse(LOC("PVEHNTFND"));
				}
				auto model = veh.getModel();
				if (!CommandAnyVehRc::is_enabled
					&& !model.isRemoteControllablePv()
					)
				{
					return click.setResponse(LOC("GTARC_E"));
				}
				click.ensureScriptThread([model]
				{
					ScriptGlobal(GLOBAL_RC_PV_MODEL).set<int>(model);
				});
			});
			cur_pv->createChild<CommandLambdaAction>(LOC("RETSTOR"), { CMDNAME("returnpv") }, NOLABEL, [](Click& click)
			{
				if (!click.inOnline())
				{
					return;
				}
				if (!AbstractEntity::getPersonalVehicle().isValid())
				{
					return click.setResponse(LOC("PVEHNTFND"));
				}
				click.ensureScriptThread([]
				{
					*ScriptGlobal(GLOBAL_PVS).at(ScriptGlobal(GLOBAL_MOST_RECENT_PV_INDEX).get<int>(), GLOBAL_PVS_ELMSIZE).at(GLOBAL_PVS_INSURANCE).as<int*>() &= ~(1 << GLOBAL_PVS_INSURANCE_OUT_GARAGE);
				});
			});
		}
		{
			auto last_veh = this->createChild<CommandListMyVehicle>(false);
			last_veh->createChild<CommandRc>();
			last_veh->createChild<CommandVehFollow>();
		}

		this->createChild<CommandCustoms>();

		// Movement
		{
			auto movement = this->createChild<CommandList>(LOC("MVMNT"));

			// Super Drive
			{
				auto superdrive = movement->createChild<CommandList>(LOC("SPRDRV"));

				auto superdrive_cam_toggle = superdrive->makeChild<CommandToggle>(LOC("SPRDRVCAM"), { CMDNAME("superdrivecam") }, LOC("SPRDRVCAM_H"), true);
				auto superdrive_speed = superdrive->makeChild<CommandSliderFloat>(LOC("SPD"), { CMDNAME("superdrivespeed") }, NOLABEL, -1000000, 1000000, 300, 10);
				auto controller_key = superdrive->makeChild<CommandSelectControllerKey>(LOC("CK"), CMDNAMES("superdrivecontrollerbutton"), INPUT_FRONTEND_RT);
				superdrive->createChild<CommandSuperDrivePresets>();
				superdrive->createChild<CommandSuperDrive>(superdrive_cam_toggle.get(), superdrive_speed.get(), controller_key.get());
				superdrive->children.emplace_back(std::move(superdrive_cam_toggle));
				superdrive->children.emplace_back(std::move(superdrive_speed));
				superdrive->children.emplace_back(std::move(controller_key));
			}
			movement->createChild<CommandSuperHandbrake>();
			movement->createChild<CommandDriveOnWater>();
			movement->createChild<CommandWaterwheels>();
			movement->createChild<CommandSpeedLimit>();
			movement->createChild<CommandListHandling>();
			movement->createChild<CommandSpecialFlight>();
			movement->createChild<CommandVehicleModelFlag>(CVehicleModelInfo::FLAG_JUMPING_CAR, LOC("JMPABL"), { CMDNAME("vehjump") }, LOC("JMPABL_H"));
			movement->createChild<CommandVehicleModelFlag>(CVehicleModelInfo::FLAG_HAS_GLIDER, LOC("GLDABL"), CMDNAMES("vehglide"));
			movement->createChild<CommandNoTurbulence>();
			movement->createChild<CommandDisableHeliAutoStabilisation>();

			// Gravity Multiplier
			{
				auto list = movement->createChild<CommandList>(LOC("GRVTYMTPLY"));
				auto grav = list->createChild<CommandVehicleGravityMultiplier>();
				grav->not_in_aircraft = list->createChild<CommandToggle>(LOC("NOTAIR"), CMDNAMES_OBF("gravitynotinaircraft"));
			}

			movement->createChild<CommandVehicleEnginePowerMultiplier>();
			movement->createChild<CommandCustomGlider>();
			movement->createChild<CommandLessenTyreBurnouts>();
			movement->createChild<CommandSmoothCoasting>();
			movement->createChild<CommandDriftMode>();
		}

		// Rocket Boost
		{
			auto rocket_boost = this->createChild<CommandList>(LOC("RKTBST"));

			rocket_boost->createChild<CommandVehicleModelFlag>(CVehicleModelInfo::FLAG_HAS_ROCKET_BOOST, LOC("OVRD"), CMDNAMES("vehrocket", "vehboost", "vehrocketboost"), LOC("RKTBST_H_OVRD"));
			rocket_boost->createChild<CommandVehicleModelFlag>(CVehicleModelInfo::FLAG_HAS_VERTICAL_ROCKET_BOOST, LOC("VERTRKTBST"), CMDNAMES("verticalboost"));
			rocket_boost->createChild<CommandBoostMod>();
			rocket_boost->createChild<CommandAutoBoost>();
		}

		// Collisions
		{
			auto collisions = this->createChild<CommandList>(LOC("COLSNS"));

			collisions->createChild<CommandVehicleModelFlag>(CVehicleModelInfo::FLAG_RAMP, LOC("RMP"), CMDNAMES("vehramp"));
			collisions->createChild<CommandVehicleModelFlag>(CVehicleModelInfo::FLAG_RAMMING_SCOOP, LOC("RMMSCOOP"), CMDNAMES("vehscoop"));
			collisions->createChild<CommandVehicleModelFlag>(CVehicleModelInfo::FLAG_HAS_INCREASED_RAMMING_FORCE, LOC("INCRMMFRC"), CMDNAMES("increasedrammingforce"));
			collisions->createChild<CommandGhostVehicles>();
			collisions->createChild<CommandVehicleNoPedCollision>();
			collisions->createChild<CommandVehicleNoObjectCollision>();
			collisions->createChild<CommandVehicleNoCollision>();
		}

		// Doors
		{
			auto doors = this->createChild<CommandList>(LOC("VDO"));

			doors->createChild<CommandVehDoorOpen>(-1, LOC("VEHDOOR_A"));
			doors->createChild<CommandVehDoorOpen>(0, LOC("VEHDOOR_0"));
			doors->createChild<CommandVehDoorOpen>(1, LOC("VEHDOOR_1"));
			doors->createChild<CommandVehDoorOpen>(2, LOC("VEHDOOR_2"));
			doors->createChild<CommandVehDoorOpen>(3, LOC("VEHDOOR_3"));
			doors->createChild<CommandVehDoorOpen>(4, LOC("VEHDOOR_4"));
			doors->createChild<CommandVehDoorOpen>(5, LOC("VEHDOOR_5"));
			doors->createChild<CommandVehDoorOpen>(6, LOC("VEHDOOR_6"));
			doors->createChild<CommandVehDoorOpen>(7, LOC("VEHDOOR_7"));
		}

		// Lock Doors
		{
			auto lck = this->createChild<CommandList>(LOC("LOCKD"));
			auto cmd = lck->createChild<CommandLockDoors>();
			cmd->only_when_vacant = lck->createChild<CommandToggle>(LOC("LOCKD_O"), CMDNAMES_OBF("lockwhenvacant"));
			cmd->exclude_others_personal_vehicles = lck->createChild<CommandToggle>(LOC("XCLUDEPVS"), CMDNAMES_OBF("excludeotherslock"));

			lck->createChild<CommandDivider>(LOC("XCLDS"));

			cmd->excludes.populateList(lck, {}, PlayerExcludes::SHOW_ORG_MEMBERS);
			cmd->exclude_missions = lck->createChild<CommandToggle>(LOC("XCLDMIS"), {}, NOLABEL, true);
		}

		// AR Speedometer
		{
			auto arspeed = this->createChild<CommandList>(LOC("ARSPD"), {}, LOC("ARSPD_H"));

			auto colour = arspeed->makeChild<CommandColour>(LOC("CLR"), { CMDNAME("arspeed") }, NOLABEL, 255, 0, 255, 255);
			auto alt_rendering = arspeed->makeChild<CommandToggle>(LOC("RNDR_NTV"), { CMDNAME("arspeednative") });
			arspeed->createChild<CommandArSpeed>(colour.get(), alt_rendering.get());
			auto colour_ = colour.get();
			arspeed->children.emplace_back(std::move(colour));
			arspeed->children.emplace_back(std::move(alt_rendering));
			create_rainbow(colour_);
			colour_->command_names.at(0).append(CMDNAME("hex"));
		}

		this->createChild<CommandCountermeasures>();
		this->createChild<CommandAutodrive>();
		this->createChild<CommandLightSignals>();
		this->createChild<CommandFlip>();

		this->createChild<CommandFixVehicle>();
		this->createChild<CommandVehGod>();
		this->createChild<CommandMint>();
		this->createChild<CommandVehInvisibility>();
		this->createChild<CommandVehicleHeadlightsIntensity>();

		this->createChild<CommandNoLockon>();
		this->createChild<CommandDontJackMe>();

		this->createChild<CommandInstaSeat>();
		this->createChild<CommandExitStop>();
		this->createChild<CommandLeaveEngineRunning>();

		{
			auto autosubtrans = this->createChild<CommandList>(LOC("ATRNSUBCAR"));

			auto main = autosubtrans->createChild<CommandSubCarAutoSwitch>();
			main->config_noAnim = autosubtrans->createChild<CommandToggle>(LOC("NANIM"));
		}

		this->createChild<CommandDrivebyInAny>();
		this->createChild<CommandDrivebyWithAny>();
		this->createChild<CommandWheelOnWheels>();
		this->createChild<CommandBombTrajectory>();

		this->createChild<CommandFlyThroughWindscreen>();
		this->createChild<CommandLockOnToAllVehicles>();
		this->createChild<CommandVehicleSeat>();

		this->createChild<CommandPegasus>();
		this->createChild<CommandForgeModel>();

		this->createChild<CommandToggleEngine>();
		this->createChild<CommandVehicleDestroy>();
		this->createChild<CommandDelete>();
	}
}
