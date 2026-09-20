#include "Scripting/SessionScriptMgr.hpp"

#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"
#include "Scripting/eMpMission.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Scripting/ScriptGlobal.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/Scripts.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	bool SessionScriptMgr::startPrecheck(Click& click)
	{
		if (!*Pointers.IsSessionStarted
			|| Scripts::FindScriptThread("am_launcher"_J) == nullptr
			)
		{
			click.setResponse(LOC("CMDONL"));
			return false;
		}
		return true;
	}

	static void setScript(const hash_t hash)
	{
		*ScriptGlobal(GLOBAL_AM_LAUNCHER_HOST_DATA + 3).as<int*>() = 0;
		*ScriptGlobal(GLOBAL_AM_LAUNCHER_HOST_DATA + 3 + 1).as<int*>() = SessionScriptMgr::hash_to_id(hash);
	}

	void SessionScriptMgr::start(const hash_t hash)
	{
		auto thread = Scripts::FindScriptThread("am_launcher"_J);
		if (!thread)
		{
			return;
		}

		if (busy)
		{
			return;
		}
		busy = true;

		Scripts::ForceScriptHost(thread);

		*ScriptGlobal(GLOBAL_AM_LAUNCHER_HOST_DATA).as<int*>() = 1;

		{
			time_t time = get_current_time_millis();
			while (thread->m_Stack)
			{
				bool all_ready = true;
				for (int p = 0; p < 32; ++p)
				{
					if (p == PLAYER::PLAYER_ID() || !NETWORK::NETWORK_IS_PLAYER_CONNECTED(p))
						continue;

					auto state = ScriptLocal(thread, LOCAL_LNCH_PLAYERSTATE).at(p, 3).at(2).as<int*>();
					if (*state == 4 || *state == 6)
					{
						if (!(*state & (1 << 2)))
						{
							setScript(static_cast<hash_t>("scroll_arcade_cabinet"_J));
						}
						else
						{
							setScript(static_cast<hash_t>("am_launcher"_J));
						}
						*ScriptGlobal(GLOBAL_AM_LAUNCHER_HOST_DATA + 2).as<int*>() = 7;

						all_ready = false;
						break;
					}
				}
				if (all_ready || GET_MILLIS_SINCE(time) > 3000)
				{
					break;
				}
				Script::current()->yield();
			}
		}

		*ScriptGlobal(GLOBAL_AM_LAUNCHER_HOST_DATA + 1).as<int*>() |= (1 << 1);
		setScript(hash);
		*ScriptGlobal(GLOBAL_AM_LAUNCHER_HOST_DATA + 2).as<int*>() = 4;

		busy = false;
	}

	int SessionScriptMgr::hash_to_id(const hash_t hash)
	{
		for (int i = 1; i < NUM_SCRIPT_IDS; ++i)
		{
			if (id_to_hash(i) == hash)
			{
				return i;
			}
		}
		return 0;
	}

	hash_t SessionScriptMgr::id_to_hash(const int id)
	{
		switch (id)
		{
		case AM_HOLD_UP: return static_cast<hash_t>("AM_HOLD_UP"_J);
		case AM_JOYRIDER: return static_cast<hash_t>("AM_JOYRIDER"_J);
		case AM_PLANE_TAKEDOWN: return static_cast<hash_t>("AM_PLANE_TAKEDOWN"_J);
		case AM_DISTRACT_COPS: return static_cast<hash_t>("AM_DISTRACT_COPS"_J);
		case AM_DESTROY_VEH: return static_cast<hash_t>("AM_DESTROY_VEH"_J);
		case AM_HOT_TARGET: return static_cast<hash_t>("AM_HOT_TARGET"_J);
		case AM_KILL_LIST: return static_cast<hash_t>("AM_KILL_LIST"_J);
		case AM_TIME_TRIAL: return static_cast<hash_t>("AM_TIME_TRIAL"_J);
		case AM_CP_COLLECTION: return static_cast<hash_t>("AM_CP_COLLECTION"_J);
		case AM_CHALLENGES: return static_cast<hash_t>("AM_CHALLENGES"_J);
		case AM_PENNED_IN: return static_cast<hash_t>("AM_PENNED_IN"_J);
		case AM_PASS_THE_PARCEL: return static_cast<hash_t>("AM_PASS_THE_PARCEL"_J);
		case AM_HOT_PROPERTY: return static_cast<hash_t>("AM_HOT_PROPERTY"_J);
		case AM_DEAD_DROP: return static_cast<hash_t>("AM_DEAD_DROP"_J);
		case AM_KING_OF_THE_CASTLE: return static_cast<hash_t>("AM_KING_OF_THE_CASTLE"_J);
		case AM_CRIMINAL_DAMAGE: return static_cast<hash_t>("AM_CRIMINAL_DAMAGE"_J);
		case AM_HUNT_THE_BEAST: return static_cast<hash_t>("AM_HUNT_THE_BEAST"_J);
		case GB_LIMO_ATTACK: return static_cast<hash_t>("GB_LIMO_ATTACK"_J);
		case GB_DEATHMATCH: return static_cast<hash_t>("GB_DEATHMATCH"_J);
		case GB_STEAL_VEH: return static_cast<hash_t>("GB_STEAL_VEH"_J);
		case GB_POINT_TO_POINT: return static_cast<hash_t>("GB_POINT_TO_POINT"_J);
		case GB_TERMINATE: return static_cast<hash_t>("GB_TERMINATE"_J);
		case GB_YACHT_ROB: return static_cast<hash_t>("GB_YACHT_ROB"_J);
		case GB_BELLYBEAST: return static_cast<hash_t>("GB_BELLYBEAST"_J);
		case GB_FIVESTAR: return static_cast<hash_t>("GB_FIVESTAR"_J);
		case GB_ROB_SHOP: return static_cast<hash_t>("GB_ROB_SHOP"_J);
		case GB_COLLECT_MONEY: return static_cast<hash_t>("GB_COLLECT_MONEY"_J);
		case GB_ASSAULT: return static_cast<hash_t>("GB_ASSAULT"_J);
		case GB_VEH_SURV: return static_cast<hash_t>("GB_VEH_SURV"_J);
		case GB_SIGHTSEER: return static_cast<hash_t>("GB_SIGHTSEER"_J);
		case GB_FLYING_IN_STYLE: return static_cast<hash_t>("GB_FLYING_IN_STYLE"_J);
		case GB_FINDERSKEEPERS: return static_cast<hash_t>("GB_FINDERSKEEPERS"_J);
		case GB_HUNT_THE_BOSS: return static_cast<hash_t>("GB_HUNT_THE_BOSS"_J);
		case GB_CARJACKING: return static_cast<hash_t>("GB_CARJACKING"_J);
		case GB_HEADHUNTER: return static_cast<hash_t>("GB_HEADHUNTER"_J);
		case GB_CONTRABAND_BUY: return static_cast<hash_t>("GB_CONTRABAND_BUY"_J);
		case GB_CONTRABAND_SELL: return static_cast<hash_t>("GB_CONTRABAND_SELL"_J);
		case GB_CONTRABAND_DEFEND: return static_cast<hash_t>("GB_CONTRABAND_DEFEND"_J);
		case GB_AIRFREIGHT: return static_cast<hash_t>("GB_AIRFREIGHT"_J);
		case GB_CASHING_OUT: return static_cast<hash_t>("GB_CASHING_OUT"_J);
		case GB_SALVAGE: return static_cast<hash_t>("GB_SALVAGE"_J);
		case GB_FRAGILE_GOODS: return static_cast<hash_t>("GB_FRAGILE_GOODS"_J);
		case dont_cross_the_line: return static_cast<hash_t>("dont_cross_the_line"_J);
		case grid_arcade_cabinet: return static_cast<hash_t>("grid_arcade_cabinet"_J);
		case scroll_arcade_cabinet: return static_cast<hash_t>("scroll_arcade_cabinet"_J);
		case example_arcade: return static_cast<hash_t>("example_arcade"_J);
		case road_arcade: return static_cast<hash_t>("road_arcade"_J);
		case Degenatron_Games: return static_cast<hash_t>("Degenatron Games"_J);
		case gunslinger_arcade: return static_cast<hash_t>("gunslinger_arcade"_J);
		case ggsm_arcade: return static_cast<hash_t>("ggsm_arcade"_J);
		case wizard_arcade: return static_cast<hash_t>("wizard_arcade"_J);
		case AM_CASINO_LIMO: return static_cast<hash_t>("AM_CASINO_LIMO"_J);
		case AM_CASINO_LUXURY_CAR: return static_cast<hash_t>("AM_CASINO_LUXURY_CAR"_J);
		case puzzle: return static_cast<hash_t>("puzzle"_J);
		case camhedz_arcade: return static_cast<hash_t>("camhedz_arcade"_J);
		case GB_VEHICLE_EXPORT: return static_cast<hash_t>("GB_VEHICLE_EXPORT"_J);
		case GB_BIKER_JOUST: return static_cast<hash_t>("GB_BIKER_JOUST"_J);
		case GB_BIKER_RACE_P2P: return static_cast<hash_t>("GB_BIKER_RACE_P2P"_J);
		case GB_BIKER_UNLOAD_WEAPONS: return static_cast<hash_t>("GB_BIKER_UNLOAD_WEAPONS"_J);
		case GB_BIKER_BAD_DEAL: return static_cast<hash_t>("GB_BIKER_BAD_DEAL"_J);
		case GB_BIKER_RESCUE_CONTACT: return static_cast<hash_t>("GB_BIKER_RESCUE_CONTACT"_J);
		case GB_BIKER_LAST_RESPECTS: return static_cast<hash_t>("GB_BIKER_LAST_RESPECTS"_J);
		case GB_BIKER_CONTRACT_KILLING: return static_cast<hash_t>("GB_BIKER_CONTRACT_KILLING"_J);
		case GB_BIKER_CONTRABAND_SELL: return static_cast<hash_t>("GB_BIKER_CONTRABAND_SELL"_J);
		case GB_BIKER_CONTRABAND_DEFEND: return static_cast<hash_t>("GB_BIKER_CONTRABAND_DEFEND"_J);
		case GB_ILLICIT_GOODS_RESUPPLY: return static_cast<hash_t>("GB_ILLICIT_GOODS_RESUPPLY"_J);
		case GB_BIKER_DRIVEBY_ASSASSIN: return static_cast<hash_t>("GB_BIKER_DRIVEBY_ASSASSIN"_J);
		case GB_BIKER_CRIMINAL_MISCHIEF: return static_cast<hash_t>("GB_BIKER_CRIMINAL_MISCHIEF"_J);
		case GB_BIKER_RIPPIN_IT_UP: return static_cast<hash_t>("GB_BIKER_RIPPIN_IT_UP"_J);
		case GB_PLOUGHED: return static_cast<hash_t>("GB_PLOUGHED"_J);
		case GB_FULLY_LOADED: return static_cast<hash_t>("GB_FULLY_LOADED"_J);
		case GB_AMPHIBIOUS_ASSAULT: return static_cast<hash_t>("GB_AMPHIBIOUS_ASSAULT"_J);
		case GB_TRANSPORTER: return static_cast<hash_t>("GB_TRANSPORTER"_J);
		case GB_FORTIFIED: return static_cast<hash_t>("GB_FORTIFIED"_J);
		case GB_VELOCITY: return static_cast<hash_t>("GB_VELOCITY"_J);
		case GB_RAMPED_UP: return static_cast<hash_t>("GB_RAMPED_UP"_J);
		case GB_STOCKPILING: return static_cast<hash_t>("GB_STOCKPILING"_J);
		case GB_BIKER_FREE_PRISONER: return static_cast<hash_t>("GB_BIKER_FREE_PRISONER"_J);
		case GB_BIKER_SAFECRACKER: return static_cast<hash_t>("GB_BIKER_SAFECRACKER"_J);
		case GB_BIKER_STEAL_BIKES: return static_cast<hash_t>("GB_BIKER_STEAL_BIKES"_J);
		case GB_BIKER_SEARCH_AND_DESTROY: return static_cast<hash_t>("GB_BIKER_SEARCH_AND_DESTROY"_J);
		case GB_BIKER_STAND_YOUR_GROUND: return static_cast<hash_t>("GB_BIKER_STAND_YOUR_GROUND"_J);
		case GB_BIKER_DESTROY_VANS: return static_cast<hash_t>("GB_BIKER_DESTROY_VANS"_J);
		case GB_BIKER_BURN_ASSETS: return static_cast<hash_t>("GB_BIKER_BURN_ASSETS"_J);
		case GB_BIKER_SHUTTLE: return static_cast<hash_t>("GB_BIKER_SHUTTLE"_J);
		case GB_BIKER_WHEELIE_RIDER: return static_cast<hash_t>("GB_BIKER_WHEELIE_RIDER"_J);
		case GB_GUNRUNNING: return static_cast<hash_t>("GB_GUNRUNNING"_J);
		case GB_GUNRUNNING_DEFEND: return static_cast<hash_t>("GB_GUNRUNNING_DEFEND"_J);
		case GB_SMUGGLER: return static_cast<hash_t>("GB_SMUGGLER"_J);
		case GB_GANGOPS: return static_cast<hash_t>("GB_GANGOPS"_J);
		case BUSINESS_BATTLES: return static_cast<hash_t>("BUSINESS_BATTLES"_J);
		case BUSINESS_BATTLES_SELL: return static_cast<hash_t>("BUSINESS_BATTLES_SELL"_J);
		case BUSINESS_BATTLES_DEFEND: return static_cast<hash_t>("BUSINESS_BATTLES_DEFEND"_J);
		case GB_SECURITY_VAN: return static_cast<hash_t>("GB_SECURITY_VAN"_J);
		case GB_TARGET_PURSUIT: return static_cast<hash_t>("GB_TARGET_PURSUIT"_J);
		case GB_JEWEL_STORE_GRAB: return static_cast<hash_t>("GB_JEWEL_STORE_GRAB"_J);
		case GB_BANK_JOB: return static_cast<hash_t>("GB_BANK_JOB"_J);
		case GB_DATA_HACK: return static_cast<hash_t>("GB_DATA_HACK"_J);
		case GB_INFILTRATION: return static_cast<hash_t>("GB_INFILTRATION"_J);
		case GB_CASINO: return static_cast<hash_t>("GB_CASINO"_J);
		case GB_CASINO_HEIST: return static_cast<hash_t>("GB_CASINO_HEIST"_J);
		case fm_content_business_battles: return static_cast<hash_t>("fm_content_business_battles"_J);
		case fm_content_crime_scene: return static_cast<hash_t>("fm_content_crime_scene"_J);
		case fm_content_drug_vehicle: return static_cast<hash_t>("fm_content_drug_vehicle"_J);
		case fm_content_movie_props: return static_cast<hash_t>("fm_content_movie_props"_J);
		case fm_content_island_heist: return static_cast<hash_t>("fm_content_island_heist"_J);
		case fm_content_island_dj: return static_cast<hash_t>("fm_content_island_dj"_J);
		case fm_content_golden_gun: return static_cast<hash_t>("fm_content_golden_gun"_J);
		case AM_CR_SELL_DRUGS: return static_cast<hash_t>("AM_CR_SELL_DRUGS"_J);
		case AM_Safehouse: return static_cast<hash_t>("AM_Safehouse"_J);
		case MG_RACE_TO_POINT: return static_cast<hash_t>("MG_RACE_TO_POINT"_J);
		case AM_CRATE_DROP: return static_cast<hash_t>("AM_CRATE_DROP"_J);
		case AM_AMMO_DROP: return static_cast<hash_t>("AM_AMMO_DROP"_J);
		case AM_VEHICLE_DROP: return static_cast<hash_t>("AM_VEHICLE_DROP"_J);
		case AM_BRU_BOX: return static_cast<hash_t>("AM_BRU_BOX"_J);
		case AM_GA_PICKUPS: return static_cast<hash_t>("AM_GA_PICKUPS"_J);
		case AM_backup_heli: return static_cast<hash_t>("AM_backup_heli"_J);
		case AM_airstrike: return static_cast<hash_t>("AM_airstrike"_J);
		case AM_PI_MENU: return static_cast<hash_t>("AM_PI_MENU"_J);
		case AM_BOAT_TAXI: return static_cast<hash_t>("AM_BOAT_TAXI"_J);
		case AM_HELI_TAXI: return static_cast<hash_t>("AM_HELI_TAXI"_J);
		case AM_IMP_EXP: return static_cast<hash_t>("AM_IMP_EXP"_J);
		case AM_TAXI: return static_cast<hash_t>("AM_TAXI"_J);
		case AM_TAXI_LAUNCHER: return static_cast<hash_t>("AM_TAXI_LAUNCHER"_J);
		case AM_GANG_CALL: return static_cast<hash_t>("AM_GANG_CALL"_J);
		case heli_gun: return static_cast<hash_t>("heli_gun"_J);
		case am_rollercoaster: return static_cast<hash_t>("am_rollercoaster"_J);
		case am_ferriswheel: return static_cast<hash_t>("am_ferriswheel"_J);
		case AM_LAUNCHER: return static_cast<hash_t>("AM_LAUNCHER"_J);
		case AM_DAILY_OBJECTIVES: return static_cast<hash_t>("AM_DAILY_OBJECTIVES"_J);
		case AM_STRIPPER: return static_cast<hash_t>("AM_STRIPPER"_J);
		case AM_Hitchhiker: return static_cast<hash_t>("AM_Hitchhiker"_J);
		case stripclub_mp: return static_cast<hash_t>("stripclub_mp"_J);
		case AM_ArmWrestling: return static_cast<hash_t>("AM_ArmWrestling"_J);
		case AM_Tennis: return static_cast<hash_t>("AM_Tennis"_J);
		case AM_Darts: return static_cast<hash_t>("AM_Darts"_J);
		case AM_ImportExport: return static_cast<hash_t>("AM_ImportExport"_J);
		case AM_FistFight: return static_cast<hash_t>("AM_FistFight"_J);
		case AM_DropOffHooker: return static_cast<hash_t>("AM_DropOffHooker"_J);
		case AM_DOORS: return static_cast<hash_t>("AM_DOORS"_J);
		case FM_INTRO: return static_cast<hash_t>("FM_INTRO"_J);
		case AM_PROSTITUTE: return static_cast<hash_t>("AM_PROSTITUTE"_J);
		case fm_hold_up_tut: return static_cast<hash_t>("fm_hold_up_tut"_J);
		case AM_CAR_MOD_TUT: return static_cast<hash_t>("AM_CAR_MOD_TUT"_J);
		case AM_CONTACT_REQUESTS: return static_cast<hash_t>("AM_CONTACT_REQUESTS"_J);
		case am_mission_launch: return static_cast<hash_t>("am_mission_launch"_J);
		case am_npc_invites: return static_cast<hash_t>("am_npc_invites"_J);
		case am_lester_cut: return static_cast<hash_t>("am_lester_cut"_J);
		case AM_VEHICLE_SPAWN: return static_cast<hash_t>("AM_VEHICLE_SPAWN"_J);
		case am_ronTrevor_Cut: return static_cast<hash_t>("am_ronTrevor_Cut"_J);
		case AM_ARMYBASE: return static_cast<hash_t>("AM_ARMYBASE"_J);
		case AM_PRISON: return static_cast<hash_t>("AM_PRISON"_J);
		case fm_Bj_race_controler: return static_cast<hash_t>("fm_Bj_race_controler"_J);
		case fm_deathmatch_controler: return static_cast<hash_t>("fm_deathmatch_controler"_J);
		case FM_Impromptu_DM_Controler: return static_cast<hash_t>("FM_Impromptu_DM_Controler"_J);
		case fm_hideout_controler: return static_cast<hash_t>("fm_hideout_controler"_J);
		case golf_mp: return static_cast<hash_t>("golf_mp"_J);
		case Pilot_School_MP: return static_cast<hash_t>("Pilot_School_MP"_J);
		case fm_mission_controller: return static_cast<hash_t>("fm_mission_controller"_J);
		case FM_Race_Controler: return static_cast<hash_t>("FM_Race_Controler"_J);
		case Range_Modern_MP: return static_cast<hash_t>("Range_Modern_MP"_J);
		case FM_Survival_Controller: return static_cast<hash_t>("FM_Survival_Controller"_J);
		case tennis_network_mp: return static_cast<hash_t>("tennis_network_mp"_J);
		case am_heist_int: return static_cast<hash_t>("am_heist_int"_J);
		case am_lowrider_int: return static_cast<hash_t>("am_lowrider_int"_J);
		case am_darts_apartment: return static_cast<hash_t>("am_darts_apartment"_J);
		case AM_Armwrestling_Apartment: return static_cast<hash_t>("AM_Armwrestling_Apartment"_J);
		case SCTV: return static_cast<hash_t>("SCTV"_J);
		case AM_ISLAND_BACKUP_HELI: return static_cast<hash_t>("AM_ISLAND_BACKUP_HELI"_J);
		case fm_content_tuner_robbery: return static_cast<hash_t>("fm_content_tuner_robbery"_J);
		case fm_content_vehicle_list: return static_cast<hash_t>("fm_content_vehicle_list"_J);
		case tuner_sandbox_activity: return static_cast<hash_t>("tuner_sandbox_activity"_J);
		case fm_content_auto_shop_delivery: return static_cast<hash_t>("fm_content_auto_shop_delivery"_J);
		case fm_content_payphone_hit: return static_cast<hash_t>("fm_content_payphone_hit"_J);
		case fm_content_security_contract: return static_cast<hash_t>("fm_content_security_contract"_J);
		case fm_content_vip_contract_1: return static_cast<hash_t>("fm_content_vip_contract_1"_J);
		case fm_content_metal_detector: return static_cast<hash_t>("fm_content_metal_detector"_J);
		case am_agency_suv: return static_cast<hash_t>("am_agency_suv"_J);
		case fm_content_phantom_car: return static_cast<hash_t>("fm_content_phantom_car"_J);
		case fm_content_slasher: return static_cast<hash_t>("fm_content_slasher"_J);
		case fm_content_sightseeing: return static_cast<hash_t>("fm_content_sightseeing"_J);
		case fm_content_smuggler_trail: return static_cast<hash_t>("fm_content_smuggler_trail"_J);
		case fm_content_skydive: return static_cast<hash_t>("fm_content_skydive"_J);
		case fm_content_cerberus: return static_cast<hash_t>("fm_content_cerberus"_J);
		case fm_content_smuggler_plane: return static_cast<hash_t>("fm_content_smuggler_plane"_J);
		case fm_content_parachuter: return static_cast<hash_t>("fm_content_parachuter"_J);
		case fm_content_bar_resupply: return static_cast<hash_t>("fm_content_bar_resupply"_J);
		case fm_content_bike_shop_delivery: return static_cast<hash_t>("fm_content_bike_shop_delivery"_J);
		case fm_content_clubhouse_contracts: return static_cast<hash_t>("fm_content_clubhouse_contracts"_J);
		case fm_content_cargo: return static_cast<hash_t>("fm_content_cargo"_J);
		case fm_content_export_cargo: return static_cast<hash_t>("fm_content_export_cargo"_J);
		case fm_content_ammunation: return static_cast<hash_t>("fm_content_ammunation"_J);
		case fm_content_gunrunning: return static_cast<hash_t>("fm_content_gunrunning"_J);
		case fm_content_source_research: return static_cast<hash_t>("fm_content_source_research"_J);
		case fm_content_club_management: return static_cast<hash_t>("fm_content_club_management"_J);
		case fm_content_club_odd_jobs: return static_cast<hash_t>("fm_content_club_odd_jobs"_J);
		case fm_content_club_source: return static_cast<hash_t>("fm_content_club_source"_J);
		case fm_content_convoy: return static_cast<hash_t>("fm_content_convoy"_J);
		case fm_content_robbery: return static_cast<hash_t>("fm_content_robbery"_J);
		case fm_content_acid_lab_setup: return static_cast<hash_t>("fm_content_acid_lab_setup"_J);
		case fm_content_acid_lab_source: return static_cast<hash_t>("fm_content_acid_lab_source"_J);
		case fm_content_acid_lab_sell: return static_cast<hash_t>("fm_content_acid_lab_sell"_J);
		case fm_content_drug_lab_work: return static_cast<hash_t>("fm_content_drug_lab_work"_J);
		case fm_content_stash_house: return static_cast<hash_t>("fm_content_stash_house"_J);
		case fm_content_taxi_driver: return static_cast<hash_t>("fm_content_taxi_driver"_J);
		case fm_content_xmas_mugger: return static_cast<hash_t>("fm_content_xmas_mugger"_J);
		case fm_content_bank_shootout: return static_cast<hash_t>("fm_content_bank_shootout"_J);
		case fm_content_armoured_truck: return static_cast<hash_t>("fm_content_armoured_truck"_J);
		case fm_content_ghosthunt: return static_cast<hash_t>("fm_content_ghosthunt"_J);
		case fm_content_ufo_abduction: return static_cast<hash_t>("fm_content_ufo_abduction"_J);
		case fm_content_smuggler_sell: return static_cast<hash_t>("fm_content_smuggler_sell"_J);
		case fm_content_smuggler_resupply: return static_cast<hash_t>("fm_content_smuggler_resupply"_J);
		case fm_content_smuggler_ops: return static_cast<hash_t>("fm_content_smuggler_ops"_J);
		case fm_content_bicycle_time_trial: return static_cast<hash_t>("fm_content_bicycle_time_trial"_J);
		case fm_content_possessed_animals: return static_cast<hash_t>("fm_content_possessed_animals"_J);
		case fm_content_chop_shop_delivery: return static_cast<hash_t>("fm_content_chop_shop_delivery"_J);
		case fm_content_xmas_truck: return static_cast<hash_t>("fm_content_xmas_truck"_J);
		case fm_content_tow_truck_work: return static_cast<hash_t>("fm_content_tow_truck_work"_J);
		case fm_content_vehrob_scoping: return static_cast<hash_t>("fm_content_vehrob_scoping"_J);
		case fm_content_vehrob_task: return static_cast<hash_t>("fm_content_vehrob_task"_J);
		case fm_content_vehrob_prep: return static_cast<hash_t>("fm_content_vehrob_prep"_J);
		case fm_content_vehrob_disrupt: return static_cast<hash_t>("fm_content_vehrob_disrupt"_J);
		case fm_content_vehrob_cargo_ship: return static_cast<hash_t>("fm_content_vehrob_cargo_ship"_J);
		case fm_content_vehrob_police: return static_cast<hash_t>("fm_content_vehrob_police"_J);
		case fm_content_vehrob_arena: return static_cast<hash_t>("fm_content_vehrob_arena"_J);
		case fm_content_vehrob_casino_prize: return static_cast<hash_t>("fm_content_vehrob_casino_prize"_J);
		case fm_content_vehrob_submarine: return static_cast<hash_t>("fm_content_vehrob_submarine"_J);
		case fm_content_bounty_targets: return static_cast<hash_t>("fm_content_bounty_targets"_J);
		case fm_content_daily_bounty: return static_cast<hash_t>("fm_content_daily_bounty"_J);
		case fm_content_dispatch_work: return static_cast<hash_t>("fm_content_dispatch_work"_J);
		case fm_content_pizza_delivery: return static_cast<hash_t>("fm_content_pizza_delivery"_J);
		case fm_content_hacker_cargo_finale: return static_cast<hash_t>("fm_content_hacker_cargo_finale"_J);
		case fm_content_hacker_zancudo_fin: return static_cast<hash_t>("fm_content_hacker_zancudo_fin"_J);
		case fm_content_hacker_house_finale: return static_cast<hash_t>("fm_content_hacker_house_finale"_J);
		case fm_content_hacker_whistle_fin: return static_cast<hash_t>("fm_content_hacker_whistle_fin"_J);
		case fm_content_hacker_cargo_prep: return static_cast<hash_t>("fm_content_hacker_cargo_prep"_J);
		case fm_content_hacker_zancudo_prep: return static_cast<hash_t>("fm_content_hacker_zancudo_prep"_J);
		case fm_content_hacker_house_prep: return static_cast<hash_t>("fm_content_hacker_house_prep"_J);
		case fm_content_hacker_whistle_prep: return static_cast<hash_t>("fm_content_hacker_whistle_prep"_J);
		case fm_content_arms_trafficking: return static_cast<hash_t>("fm_content_arms_trafficking"_J);
		case AM_MP_HOTWIRE: return static_cast<hash_t>("AM_MP_HOTWIRE"_J);
		case fm_content_community_outreach: return static_cast<hash_t>("fm_content_community_outreach"_J);
		case fm_content_car_wash_work: return static_cast<hash_t>("fm_content_car_wash_work"_J);
		case fm_content_car_wash_detailing: return static_cast<hash_t>("fm_content_car_wash_detailing"_J);
		case fm_content_helitours_work: return static_cast<hash_t>("fm_content_helitours_work"_J);
		case fm_content_helitours_tour: return static_cast<hash_t>("fm_content_helitours_tour"_J);
		case fm_content_weed_shop_work: return static_cast<hash_t>("fm_content_weed_shop_work"_J);
		case fm_content_weed_shop_delivery: return static_cast<hash_t>("fm_content_weed_shop_delivery"_J);
		case fm_content_tycoon_odd_jobs: return static_cast<hash_t>("fm_content_tycoon_odd_jobs"_J);
		case fm_content_cutscene: return static_cast<hash_t>("fm_content_cutscene"_J);
		case fm_content_firefighter: return static_cast<hash_t>("fm_content_firefighter"_J);
		case fm_content_forklift_operator: return static_cast<hash_t>("fm_content_forklift_operator"_J);
		case fm_content_postal_worker: return static_cast<hash_t>("fm_content_postal_worker"_J);
		case fm_content_getaway_driver: return static_cast<hash_t>("fm_content_getaway_driver"_J);
		case fm_content_survival_grouping: return static_cast<hash_t>("fm_content_survival_grouping"_J);
		case fm_content_valentine_cheater: return static_cast<hash_t>("fm_content_valentine_cheater"_J);
		case fm_content_survival: return static_cast<hash_t>("fm_content_survival"_J);
		case fm_content_kortz_scoping: return static_cast<hash_t>("fm_content_kortz_scoping"_J);
		case fm_content_kortz_entry: return static_cast<hash_t>("fm_content_kortz_entry"_J);
		case fm_content_kortz_equipment: return static_cast<hash_t>("fm_content_kortz_equipment"_J);
		case fm_content_kortz_disruption: return static_cast<hash_t>("fm_content_kortz_disruption"_J);
		case fm_content_kortz_task: return static_cast<hash_t>("fm_content_kortz_task"_J);
		}
		return 0;
	}
}
