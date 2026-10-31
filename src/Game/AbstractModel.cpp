#include "Game/AbstractModel.hpp"

#include "Game/CBaseModelInfo.hpp"
#include "Game/CVehicleModelInfo.hpp"
#include "Core/Hooking.hpp"
#include "Util/atStringHash.hpp"
#include "Util/joaatToString.hpp"
#include "Util/Util.hpp"
#include "Scripting/Natives.hpp"

namespace Stand
{
	CBaseModelInfo* AbstractModel::getModelInfo() const noexcept
	{
		return g_hooking.getModelInfo(model);
	}

	CVehicleModelInfo* AbstractModel::getVehicleModelInfo() const noexcept
	{
		if (auto mi = getModelInfo())
		{
			if (mi->GetModelType() == MI_TYPE_VEHICLE)
			{
				return reinterpret_cast<CVehicleModelInfo*>(mi);
			}
		}
		return nullptr;
	}

	VehicleType AbstractModel::getVehicleType() const noexcept
	{
		if (auto info = getVehicleModelInfo())
		{
			return info->vehicle_type;
		}
		return (VehicleType)-1;
	}

	bool AbstractModel::isValidModel() const
	{
		return STREAMING::IS_MODEL_VALID(model);
	}

	bool AbstractModel::isNull() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("slod_human"):
		case ATSTRINGHASH_UNS("slod_large_quadped"):
		case ATSTRINGHASH_UNS("slod_small_quadped"):
			return true;
		}
		return isNullVehicle();
	}

	bool AbstractModel::isNullVehicle() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("arbitergt"):
		case ATSTRINGHASH_UNS("astron2"):
		case ATSTRINGHASH_UNS("cyclone2"):
		case ATSTRINGHASH_UNS("ignus2"):
		case ATSTRINGHASH_UNS("s95"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isPed() const noexcept
	{
		auto info = getModelInfo();
		return info && info->m_type == MI_TYPE_PED;
	}

	bool AbstractModel::isObject() const noexcept
	{
		auto info = getModelInfo();
		if (info == nullptr)
		{
			return true;
		}
		auto type = info->GetModelType();
		return type == MI_TYPE_BASE
			|| type == MI_TYPE_WEAPON
			;
	}

	bool AbstractModel::isObjectStrict() const noexcept
	{
		if (auto info = getModelInfo())
		{
			auto type = info->GetModelType();
			return type == MI_TYPE_BASE
				|| type == MI_TYPE_WEAPON
				;
		}
		return false;
	}

	bool AbstractModel::isVehicle() const noexcept
	{
		auto info = getModelInfo();
		return info && info->m_type == MI_TYPE_VEHICLE;
	}

	Vector3 AbstractModel::getDimensions() const
	{
		Vector3 min;
		Vector3 dimensions;
		MISC::GET_MODEL_DIMENSIONS(model, &min, &dimensions);
		dimensions.x -= min.x;
		dimensions.y -= min.y;
		dimensions.z -= min.z;
		return dimensions;
	}

	Label AbstractModel::getLabel() const
	{
		if (auto obj = joaatToStringRaw(model))
		{
			return LIT(obj);
		}
		return LIT(Util::to_padded_hex_string_with_0x(model));
	}

	Label AbstractModel::getLabelPed() const
	{
		return LIT(Util::to_padded_hex_string_with_0x(model));
	}

	bool AbstractModel::isSubmarine() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_SUBMARINE;
	}

	bool AbstractModel::isTrailer() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_TRAILER;
	}

	bool AbstractModel::isPlane() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_PLANE;
	}

	bool AbstractModel::isHeli() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_HELI;
	}

	bool AbstractModel::isBike() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_BIKE;
	}

	bool AbstractModel::isBoat() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_BOAT;
	}

	bool AbstractModel::isBicycle() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_BICYCLE;
	}

	bool AbstractModel::isSubmarineCar() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_SUBMARINECAR;
	}

	bool AbstractModel::isTrain() const noexcept
	{
		return getVehicleType() == VEHICLE_TYPE_TRAIN;
	}

	bool AbstractModel::isAirborne() const noexcept
	{
		switch (getVehicleType())
		{
		case VEHICLE_TYPE_HELI:
		case VEHICLE_TYPE_BLIMP:
		case VEHICLE_TYPE_PLANE:
			return true;
		default:
			break;
		}
		return false;
	}

	bool AbstractModel::isAquatic() const noexcept
	{
		switch (getVehicleType())
		{
		case VEHICLE_TYPE_BOAT:
		case VEHICLE_TYPE_SUBMARINE:
			return true;
		default:
			break;
		}
		return false;
	}

	int AbstractModel::getNumSeats() const
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("cheburek"): return 4;
		case ATSTRINGHASH_UNS("issi8"): return 4;
		case ATSTRINGHASH_UNS("kamacho"): return 4;
		case ATSTRINGHASH_UNS("rhinehart"): return 4;
		case ATSTRINGHASH_UNS("seminole2"): return 4;
		case ATSTRINGHASH_UNS("stafford"): return 4;
		case ATSTRINGHASH_UNS("sultan2"): return 4;
		case ATSTRINGHASH_UNS("trailerlarge"): return 3;
		}
		return VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(model);
	}

	bool AbstractModel::isTank() const noexcept
	{
		return model == ATSTRINGHASH("rhino") || model == ATSTRINGHASH("khanjali");
	}

	bool AbstractModel::isTraffic() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("radi"):
		case ATSTRINGHASH_UNS("intruder"):
		case ATSTRINGHASH_UNS("granger"):
		case ATSTRINGHASH_UNS("cavalcade2"):
		case ATSTRINGHASH_UNS("premier"):
		case ATSTRINGHASH_UNS("pcj"):
		case ATSTRINGHASH_UNS("akuma"):
		case ATSTRINGHASH_UNS("futo"):
		case ATSTRINGHASH_UNS("minivan"):
		case ATSTRINGHASH_UNS("rancherxl"):
		case ATSTRINGHASH_UNS("buccaneer"):
		case ATSTRINGHASH_UNS("bullet"):
		case ATSTRINGHASH_UNS("bagger"):
		case ATSTRINGHASH_UNS("felon2"):
		case ATSTRINGHASH_UNS("baller2"):
		case ATSTRINGHASH_UNS("feltzer2"):
		case ATSTRINGHASH_UNS("ruiner"):
		case ATSTRINGHASH_UNS("carbonizzare"):
		case ATSTRINGHASH_UNS("schafter2"):
		case ATSTRINGHASH_UNS("serrano"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isTowTruck() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("towtruck"):
		case ATSTRINGHASH_UNS("towtruck2"):
		case ATSTRINGHASH_UNS("towtruck3"):
		case ATSTRINGHASH_UNS("towtruck4"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isCargobob() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("cargobob"):
		case ATSTRINGHASH_UNS("cargobob2"):
		case ATSTRINGHASH_UNS("cargobob3"):
		case ATSTRINGHASH_UNS("cargobob4"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isBigVehicle() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("alkonost"):
		case ATSTRINGHASH_UNS("avenger2"):
		case ATSTRINGHASH_UNS("dump"):
		case ATSTRINGHASH_UNS("blimp"):
		case ATSTRINGHASH_UNS("blimp2"):
		case ATSTRINGHASH_UNS("blimp3"):
		case ATSTRINGHASH_UNS("bombushka"):
		case ATSTRINGHASH_UNS("cargoplane"):
		case ATSTRINGHASH_UNS("cargoplane2"):
		case ATSTRINGHASH_UNS("hydra"):
		case ATSTRINGHASH_UNS("jet"):
		case ATSTRINGHASH_UNS("tug"):
		case ATSTRINGHASH_UNS("volatol"):
		case ATSTRINGHASH_UNS("tr2"):
		case ATSTRINGHASH_UNS("tr3"):
		case ATSTRINGHASH_UNS("tr4"):
			return true;
		}
		return false;
	}

	bool AbstractModel::hasSpecialFlight() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("deluxo"):
		case ATSTRINGHASH_UNS("oppressor2"):
			return true;
		}
		return false;
	}

	bool AbstractModel::hasVtol() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("hydra"):
		case ATSTRINGHASH_UNS("tula"):
		case ATSTRINGHASH_UNS("avenger"):
		case ATSTRINGHASH_UNS("avenger2"):
		case ATSTRINGHASH_UNS("avenger3"):
		case ATSTRINGHASH_UNS("avenger4"):
		case ATSTRINGHASH_UNS("raiju"):
			return true;
		}
		return false;
	}

	bool AbstractModel::hasDriftTune() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("drifteuros"):
		case ATSTRINGHASH_UNS("driftzr350"):
		case ATSTRINGHASH_UNS("driftfr36"):
		case ATSTRINGHASH_UNS("driftfuto"):
		case ATSTRINGHASH_UNS("driftjester"):
		case ATSTRINGHASH_UNS("driftyosemite"):
		case ATSTRINGHASH_UNS("driftremus"):
		case ATSTRINGHASH_UNS("drifttampa"):
			return true;
		}
		return false;
	}

	bool AbstractModel::hasWindowTintMinusOne() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("granger"):
		case ATSTRINGHASH_UNS("visione"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isStealthVehicle() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("akula"):
		case ATSTRINGHASH_UNS("alkonost"):
		case ATSTRINGHASH_UNS("annihilator2"):
		case ATSTRINGHASH_UNS("kosatka"):
		case ATSTRINGHASH_UNS("microlight"):
		case ATSTRINGHASH_UNS("raiju"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isRemoteControllable() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("rcbandito"):
		case ATSTRINGHASH_UNS("minitank"):
			return true;
		}
		return isRemoteControllablePv();
	}

	bool AbstractModel::isRemoteControllablePv() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("deity"):
		case ATSTRINGHASH_UNS("granger2"):
		case ATSTRINGHASH_UNS("buffalo4"):
		case ATSTRINGHASH_UNS("jubilee"):
		case ATSTRINGHASH_UNS("patriot3"):
		case ATSTRINGHASH_UNS("champion"):
		case ATSTRINGHASH_UNS("greenwood"):
		case ATSTRINGHASH_UNS("omnisegt"):
		case ATSTRINGHASH_UNS("virtue"):
		case ATSTRINGHASH_UNS("r300"):
			return true;
		}
		return false;
	}

	bool AbstractModel::usesAttachmentCameras() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("terbyte"):
		case ATSTRINGHASH_UNS("avenger"):
		case ATSTRINGHASH_UNS("avenger2"):
		case ATSTRINGHASH_UNS("avenger3"):
		case ATSTRINGHASH_UNS("avenger4"):
		case ATSTRINGHASH_UNS("kosatka"):
			return true;
		}
		return false;
	}

	bool AbstractModel::canBeStoodUpon() const noexcept
	{
		if (auto mi = getVehicleModelInfo())
		{
			return mi->flags.get(CVehicleModelInfo::FLAG_PEDS_CAN_STAND_ON_TOP);
		}
		return false;
	}

	bool AbstractModel::isCageObject() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("prop_gold_cont_01"):
		case ATSTRINGHASH_UNS("prop_rub_cage01a"):
		case ATSTRINGHASH_UNS("prop_fnclink_03e"):
		case ATSTRINGHASH_UNS("stt_prop_stunt_tube_s"):
		case ATSTRINGHASH_UNS("mule"):
		case ATSTRINGHASH_UNS("boxville"):
		case ATSTRINGHASH_UNS("boxville2"):
		case ATSTRINGHASH_UNS("boxville3"):
		case ATSTRINGHASH_UNS("boxville4"):
		case ATSTRINGHASH_UNS("boxville5"):
		case ATSTRINGHASH_UNS("prop_gascage01"):
		case ATSTRINGHASH_UNS("hei_prop_heist_apecrate"):
		case ATSTRINGHASH_UNS("prop_container_05a"):
		case ATSTRINGHASH_UNS("prop_container_ld_pu"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isScubaObject() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("p_d_scuba_mask_s"):
		case ATSTRINGHASH_UNS("p_d_scuba_tank_s"):
		case ATSTRINGHASH_UNS("p_michael_scuba_mask_s"):
		case ATSTRINGHASH_UNS("p_michael_scuba_tank_s"):
		case ATSTRINGHASH_UNS("p_s_scuba_mask_s"):
		case ATSTRINGHASH_UNS("p_s_scuba_tank_s"):
		case ATSTRINGHASH_UNS("p_steve_scuba_hood_s"):
		case ATSTRINGHASH_UNS("xm_prop_x17_scuba_tank"):
		case ATSTRINGHASH_UNS("xm_int_lev_scuba_gear"):
		case ATSTRINGHASH_UNS("h4_int_sub_scuba_gear_002"):
		case ATSTRINGHASH_UNS("h4_int_sub_scuba_gear_01"):
		case ATSTRINGHASH_UNS("h4_int_lev_scuba_gear"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isStoryCharacter() const noexcept
	{
		return model == ATSTRINGHASH("player_zero") || model == ATSTRINGHASH("player_one") || model == ATSTRINGHASH("player_two");
	}

	bool AbstractModel::isOnlineCharacter() const noexcept
	{
		return model == ATSTRINGHASH("mp_m_freemode_01") || model == ATSTRINGHASH("mp_f_freemode_01");
	}

	bool AbstractModel::isPlayableOnlineCharacter() const noexcept
	{
		return isOnlineCharacter() || model == ATSTRINGHASH("p_franklin_02") || model == ATSTRINGHASH("ig_lamardavis_02");
	}

	bool AbstractModel::isAnimal() const noexcept
	{
		return false;
	}

	bool AbstractModel::isWaterAnimal() const noexcept
	{
		return false;
	}

	bool AbstractModel::isObtainableFromPeyote() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("a_c_boar"):
		case ATSTRINGHASH_UNS("a_c_cat_01"):
		case ATSTRINGHASH_UNS("a_c_cow"):
		case ATSTRINGHASH_UNS("a_c_coyote"):
		case ATSTRINGHASH_UNS("a_c_deer"):
		case ATSTRINGHASH_UNS("a_c_husky"):
		case ATSTRINGHASH_UNS("a_c_mtlion"):
		case ATSTRINGHASH_UNS("a_c_pig"):
		case ATSTRINGHASH_UNS("a_c_poodle"):
		case ATSTRINGHASH_UNS("a_c_pug"):
		case ATSTRINGHASH_UNS("a_c_rabbit_01"):
		case ATSTRINGHASH_UNS("a_c_retriever"):
		case ATSTRINGHASH_UNS("a_c_rottweiler"):
		case ATSTRINGHASH_UNS("a_c_shepherd"):
		case ATSTRINGHASH_UNS("a_c_westy"):
		case ATSTRINGHASH_UNS("a_c_chickenhawk"):
		case ATSTRINGHASH_UNS("a_c_cormorant"):
		case ATSTRINGHASH_UNS("a_c_crow"):
		case ATSTRINGHASH_UNS("a_c_hen"):
		case ATSTRINGHASH_UNS("a_c_pigeon"):
		case ATSTRINGHASH_UNS("a_c_seagull"):
		case ATSTRINGHASH_UNS("a_c_dolphin"):
		case ATSTRINGHASH_UNS("a_c_fish"):
		case ATSTRINGHASH_UNS("a_c_killerwhale"):
		case ATSTRINGHASH_UNS("a_c_sharkhammer"):
		case ATSTRINGHASH_UNS("a_c_sharktiger"):
		case ATSTRINGHASH_UNS("a_c_stingray"):
		case ATSTRINGHASH_UNS("ig_orleans"):
		case ATSTRINGHASH_UNS("a_c_chop"):
		case ATSTRINGHASH_UNS("a_c_humpback"):
			return true;
		}
		return false;
	}

	bool AbstractModel::canBecomeHostile() const noexcept
	{
		if (isAnimal())
		{
			switch (model)
			{
			case ATSTRINGHASH_UNS("a_c_chop"):
			case ATSTRINGHASH_UNS("a_c_chop_02"):
			case ATSTRINGHASH_UNS("a_c_shepherd"):
			case ATSTRINGHASH_UNS("a_c_rottweiler"):
				return true;
			}
			return false;
		}
		return true;
	}

	bool AbstractModel::canUseProjectiles() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("a_c_rat"):
		case ATSTRINGHASH_UNS("a_c_rabbit_01"):
			return false;
		}
		return !isWaterAnimal();
	}

	bool AbstractModel::canUseWeapons() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("a_c_chimp_02"):
			return true;
		}
		return !isAnimal();
	}

	bool AbstractModel::isCsPed() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("cs_amandatownley"):
		case ATSTRINGHASH_UNS("cs_andreas"):
		case ATSTRINGHASH_UNS("cs_ashley"):
		case ATSTRINGHASH_UNS("cs_bankman"):
		case ATSTRINGHASH_UNS("cs_barry"):
		case ATSTRINGHASH_UNS("cs_beverly"):
		case ATSTRINGHASH_UNS("cs_brad"):
		case ATSTRINGHASH_UNS("cs_bradcadaver"):
		case ATSTRINGHASH_UNS("cs_carbuyer"):
		case ATSTRINGHASH_UNS("cs_casey"):
		case ATSTRINGHASH_UNS("cs_chengsr"):
		case ATSTRINGHASH_UNS("cs_chrisformage"):
		case ATSTRINGHASH_UNS("cs_clay"):
		case ATSTRINGHASH_UNS("cs_dale"):
		case ATSTRINGHASH_UNS("cs_davenorton"):
		case ATSTRINGHASH_UNS("cs_debra"):
		case ATSTRINGHASH_UNS("cs_denise"):
		case ATSTRINGHASH_UNS("cs_devin"):
		case ATSTRINGHASH_UNS("cs_dom"):
		case ATSTRINGHASH_UNS("cs_dreyfuss"):
		case ATSTRINGHASH_UNS("cs_drfriedlander"):
		case ATSTRINGHASH_UNS("cs_drfriedlander_02"):
		case ATSTRINGHASH_UNS("cs_fabien"):
		case ATSTRINGHASH_UNS("cs_fbisuit_01"):
		case ATSTRINGHASH_UNS("cs_floyd"):
		case ATSTRINGHASH_UNS("cs_guadalope"):
		case ATSTRINGHASH_UNS("cs_gurk"):
		case ATSTRINGHASH_UNS("cs_hunter"):
		case ATSTRINGHASH_UNS("cs_janet"):
		case ATSTRINGHASH_UNS("cs_jewelass"):
		case ATSTRINGHASH_UNS("cs_jimmyboston"):
		case ATSTRINGHASH_UNS("cs_jimmydisanto"):
		case ATSTRINGHASH_UNS("cs_jimmydisanto2"):
		case ATSTRINGHASH_UNS("cs_joeminuteman"):
		case ATSTRINGHASH_UNS("cs_johnnyklebitz"):
		case ATSTRINGHASH_UNS("cs_josef"):
		case ATSTRINGHASH_UNS("cs_josh"):
		case ATSTRINGHASH_UNS("cs_karen_daniels"):
		case ATSTRINGHASH_UNS("cs_lamardavis"):
		case ATSTRINGHASH_UNS("cs_lamardavis_02"):
		case ATSTRINGHASH_UNS("cs_lazlow"):
		case ATSTRINGHASH_UNS("cs_lazlow_2"):
		case ATSTRINGHASH_UNS("cs_lestercrest"):
		case ATSTRINGHASH_UNS("cs_lestercrest_2"):
		case ATSTRINGHASH_UNS("cs_lestercrest_3"):
		case ATSTRINGHASH_UNS("cs_lifeinvad_01"):
		case ATSTRINGHASH_UNS("cs_magenta"):
		case ATSTRINGHASH_UNS("cs_manuel"):
		case ATSTRINGHASH_UNS("cs_marnie"):
		case ATSTRINGHASH_UNS("cs_martinmadrazo"):
		case ATSTRINGHASH_UNS("cs_maryann"):
		case ATSTRINGHASH_UNS("cs_michelle"):
		case ATSTRINGHASH_UNS("cs_milton"):
		case ATSTRINGHASH_UNS("cs_molly"):
		case ATSTRINGHASH_UNS("cs_movpremf_01"):
		case ATSTRINGHASH_UNS("cs_movpremmale"):
		case ATSTRINGHASH_UNS("cs_mrk"):
		case ATSTRINGHASH_UNS("cs_mrs_thornhill"):
		case ATSTRINGHASH_UNS("cs_mrsphillips"):
		case ATSTRINGHASH_UNS("cs_natalia"):
		case ATSTRINGHASH_UNS("cs_nervousron"):
		case ATSTRINGHASH_UNS("cs_nervousron_02"):
		case ATSTRINGHASH_UNS("cs_nigel"):
		case ATSTRINGHASH_UNS("cs_old_man1a"):
		case ATSTRINGHASH_UNS("cs_old_man2"):
		case ATSTRINGHASH_UNS("cs_omega"):
		case ATSTRINGHASH_UNS("cs_orleans"):
		case ATSTRINGHASH_UNS("cs_paper"):
		case ATSTRINGHASH_UNS("cs_patricia"):
		case ATSTRINGHASH_UNS("cs_patricia_02"):
		case ATSTRINGHASH_UNS("cs_priest"):
		case ATSTRINGHASH_UNS("cs_prolsec_02"):
		case ATSTRINGHASH_UNS("cs_russiandrunk"):
		case ATSTRINGHASH_UNS("cs_siemonyetarian"):
		case ATSTRINGHASH_UNS("cs_solomon"):
		case ATSTRINGHASH_UNS("cs_stevehains"):
		case ATSTRINGHASH_UNS("cs_stretch"):
		case ATSTRINGHASH_UNS("cs_tanisha"):
		case ATSTRINGHASH_UNS("cs_taocheng"):
		case ATSTRINGHASH_UNS("cs_taocheng2"):
		case ATSTRINGHASH_UNS("cs_taostranslator"):
		case ATSTRINGHASH_UNS("cs_taostranslator2"):
		case ATSTRINGHASH_UNS("cs_tenniscoach"):
		case ATSTRINGHASH_UNS("cs_terry"):
		case ATSTRINGHASH_UNS("cs_tom"):
		case ATSTRINGHASH_UNS("cs_tomepsilon"):
		case ATSTRINGHASH_UNS("cs_tracydisanto"):
		case ATSTRINGHASH_UNS("cs_wade"):
		case ATSTRINGHASH_UNS("cs_zimbor"):
		case ATSTRINGHASH_UNS("csb_abigail"):
		case ATSTRINGHASH_UNS("csb_agatha"):
		case ATSTRINGHASH_UNS("csb_agent"):
		case ATSTRINGHASH_UNS("csb_alan"):
		case ATSTRINGHASH_UNS("csb_anita"):
		case ATSTRINGHASH_UNS("csb_anton"):
		case ATSTRINGHASH_UNS("csb_ary"):
		case ATSTRINGHASH_UNS("csb_ary_02"):
		case ATSTRINGHASH_UNS("csb_avery"):
		case ATSTRINGHASH_UNS("csb_avischwartzman_02"):
		case ATSTRINGHASH_UNS("csb_avischwartzman_03"):
		case ATSTRINGHASH_UNS("csb_avon"):
		case ATSTRINGHASH_UNS("csb_ballas_leader"):
		case ATSTRINGHASH_UNS("csb_ballasog"):
		case ATSTRINGHASH_UNS("csb_billionaire"):
		case ATSTRINGHASH_UNS("csb_bogdan"):
		case ATSTRINGHASH_UNS("csb_bride"):
		case ATSTRINGHASH_UNS("csb_brucie2"):
		case ATSTRINGHASH_UNS("csb_bryony"):
		case ATSTRINGHASH_UNS("csb_burgerdrug"):
		case ATSTRINGHASH_UNS("csb_car3guy1"):
		case ATSTRINGHASH_UNS("csb_car3guy2"):
		case ATSTRINGHASH_UNS("csb_celeb_01"):
		case ATSTRINGHASH_UNS("csb_charlie_reed"):
		case ATSTRINGHASH_UNS("csb_chef"):
		case ATSTRINGHASH_UNS("csb_chef2"):
		case ATSTRINGHASH_UNS("CSB_Chef_03"):
		case ATSTRINGHASH_UNS("csb_chin_goon"):
		case ATSTRINGHASH_UNS("csb_cletus"):
		case ATSTRINGHASH_UNS("csb_cop"):
		case ATSTRINGHASH_UNS("csb_customer"):
		case ATSTRINGHASH_UNS("CSB_Dax"):
		case ATSTRINGHASH_UNS("csb_denise_friend"):
		case ATSTRINGHASH_UNS("csb_dix"):
		case ATSTRINGHASH_UNS("csb_djblamadon"):
		case ATSTRINGHASH_UNS("csb_drugdealer"):
		case ATSTRINGHASH_UNS("csb_englishdave"):
		case ATSTRINGHASH_UNS("csb_englishdave_02"):
		case ATSTRINGHASH_UNS("csb_fos_rep"):
		case ATSTRINGHASH_UNS("csb_g"):
		case ATSTRINGHASH_UNS("csb_georginacheng"):
		case ATSTRINGHASH_UNS("csb_golfer_a"):
		case ATSTRINGHASH_UNS("csb_golfer_b"):
		case ATSTRINGHASH_UNS("csb_groom"):
		case ATSTRINGHASH_UNS("csb_grove_str_dlr"):
		case ATSTRINGHASH_UNS("csb_gustavo"):
		case ATSTRINGHASH_UNS("csb_hao"):
		case ATSTRINGHASH_UNS("csb_hao_02"):
		case ATSTRINGHASH_UNS("csb_helmsmanpavel"):
		case ATSTRINGHASH_UNS("csb_huang"):
		case ATSTRINGHASH_UNS("csb_hugh"):
		case ATSTRINGHASH_UNS("csb_imani"):
		case ATSTRINGHASH_UNS("csb_imran"):
		case ATSTRINGHASH_UNS("csb_isldj_00"):
		case ATSTRINGHASH_UNS("csb_isldj_01"):
		case ATSTRINGHASH_UNS("csb_isldj_02"):
		case ATSTRINGHASH_UNS("csb_isldj_03"):
		case ATSTRINGHASH_UNS("csb_isldj_04"):
		case ATSTRINGHASH_UNS("csb_jackhowitzer"):
		case ATSTRINGHASH_UNS("csb_janitor"):
		case ATSTRINGHASH_UNS("csb_jio"):
		case ATSTRINGHASH_UNS("csb_jio_02"):
		case ATSTRINGHASH_UNS("csb_johnny_guns"):
		case ATSTRINGHASH_UNS("csb_juanstrickler"):
		case ATSTRINGHASH_UNS("CSB_Labrat"):
		case ATSTRINGHASH_UNS("CSB_Luchadora"):
		case ATSTRINGHASH_UNS("csb_maude"):
		case ATSTRINGHASH_UNS("csb_miguelmadrazo"):
		case ATSTRINGHASH_UNS("csb_mimi"):
		case ATSTRINGHASH_UNS("csb_mjo"):
		case ATSTRINGHASH_UNS("csb_mjo_02"):
		case ATSTRINGHASH_UNS("csb_money"):
		case ATSTRINGHASH_UNS("csb_moodyman_02"):
		case ATSTRINGHASH_UNS("csb_mp_agent14"):
		case ATSTRINGHASH_UNS("csb_mrs_r"):
		case ATSTRINGHASH_UNS("csb_musician_00"):
		case ATSTRINGHASH_UNS("csb_mweather"):
		case ATSTRINGHASH_UNS("csb_ortega"):
		case ATSTRINGHASH_UNS("csb_oscar"):
		case ATSTRINGHASH_UNS("csb_paige"):
		case ATSTRINGHASH_UNS("csb_party_promo"):
		case ATSTRINGHASH_UNS("csb_popov"):
		case ATSTRINGHASH_UNS("csb_porndudes"):
		case ATSTRINGHASH_UNS("csb_prologuedriver"):
		case ATSTRINGHASH_UNS("csb_prolsec"):
		case ATSTRINGHASH_UNS("csb_ramp_gang"):
		case ATSTRINGHASH_UNS("csb_ramp_hic"):
		case ATSTRINGHASH_UNS("csb_ramp_hipster"):
		case ATSTRINGHASH_UNS("csb_ramp_marine"):
		case ATSTRINGHASH_UNS("csb_ramp_mex"):
		case ATSTRINGHASH_UNS("csb_rashcosvki"):
		case ATSTRINGHASH_UNS("csb_reporter"):
		case ATSTRINGHASH_UNS("csb_req_officer"):
		case ATSTRINGHASH_UNS("csb_roccopelosi"):
		case ATSTRINGHASH_UNS("csb_screen_writer"):
		case ATSTRINGHASH_UNS("csb_security_a"):
		case ATSTRINGHASH_UNS("csb_sessanta"):
		case ATSTRINGHASH_UNS("csb_sol"):
		case ATSTRINGHASH_UNS("csb_soundeng_00"):
		case ATSTRINGHASH_UNS("csb_sss"):
		case ATSTRINGHASH_UNS("csb_stripper_01"):
		case ATSTRINGHASH_UNS("csb_stripper_02"):
		case ATSTRINGHASH_UNS("csb_talcc"):
		case ATSTRINGHASH_UNS("csb_talmm"):
		case ATSTRINGHASH_UNS("csb_thornton"):
		case ATSTRINGHASH_UNS("csb_tomcasino"):
		case ATSTRINGHASH_UNS("csb_tonya"):
		case ATSTRINGHASH_UNS("csb_tonyprince"):
		case ATSTRINGHASH_UNS("csb_trafficwarden"):
		case ATSTRINGHASH_UNS("csb_undercover"):
		case ATSTRINGHASH_UNS("csb_vagos_leader"):
		case ATSTRINGHASH_UNS("csb_vagspeak"):
		case ATSTRINGHASH_UNS("csb_vernon"):
		case ATSTRINGHASH_UNS("csb_vincent"):
		case ATSTRINGHASH_UNS("csb_vincent_2"):
		case ATSTRINGHASH_UNS("csb_wendy"):
			return true;
		}
		return false;
	}

	bool AbstractModel::isHiResPed() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("player_zero"):
		case ATSTRINGHASH_UNS("player_one"):
		case ATSTRINGHASH_UNS("player_two"):
		case ATSTRINGHASH_UNS("p_franklin_02"):
		case ATSTRINGHASH_UNS("ig_amandatownley"):
		case ATSTRINGHASH_UNS("ig_brad"):
		case ATSTRINGHASH_UNS("ig_josh"):
		case ATSTRINGHASH_UNS("ig_lamardavis_02"):
		case ATSTRINGHASH_UNS("ig_lazlow"):
		case ATSTRINGHASH_UNS("ig_lazlow_2"):
		case ATSTRINGHASH_UNS("ig_lestercrest"):
		case ATSTRINGHASH_UNS("ig_nervousron"):
		case ATSTRINGHASH_UNS("ig_tracydisanto"):
		case ATSTRINGHASH_UNS("ig_wade"):
			return true;
		}
		return isCsPed();
	}

	bool AbstractModel::isSusAttachee() const noexcept
	{
		if (model == ATSTRINGHASH("cargoplane2"))
		{
			return false;
		}

		switch (model)
		{
		case ATSTRINGHASH_UNS("barracks"):
		case ATSTRINGHASH_UNS("barracks3"):
		case ATSTRINGHASH_UNS("chernobog"):
		case ATSTRINGHASH_UNS("dinghy3"):
		case ATSTRINGHASH_UNS("dune"):
		case ATSTRINGHASH_UNS("scrap"):
			return true;
		}
		return isBigVehicle() || isPlane();
	}

	bool AbstractModel::isSusAttachment() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("cargobob"):
			return true;
		}
		return isSusAttachee();
	}

	bool AbstractModel::isValidParachute() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("lts_p_para_pilot2_sp_s"):
		case ATSTRINGHASH_UNS("p_parachute1_mp_dec"):
		case ATSTRINGHASH_UNS("p_parachute1_mp_s"):
		case ATSTRINGHASH_UNS("p_parachute1_s"):
		case ATSTRINGHASH_UNS("p_parachute1_sp_dec"):
		case ATSTRINGHASH_UNS("p_parachute1_sp_s"):
		case ATSTRINGHASH_UNS("pil_p_para_pilot_sp_s"):
		case ATSTRINGHASH_UNS("prop_v_parachute"):
		case ATSTRINGHASH_UNS("imp_prop_impexp_para_s"):
		case ATSTRINGHASH_UNS("sr_prop_specraces_para_s_01"):
		case ATSTRINGHASH_UNS("gr_prop_gr_para_s_01"):
		case ATSTRINGHASH_UNS("p_para_broken1_s"):
		case ATSTRINGHASH_UNS("reh_prop_reh_para_sp_s_01a"):
		case ATSTRINGHASH_UNS("tr_prop_tr_para_sp_s_01a"):
			return true;
		}
		return false;
	}

	bool AbstractModel::supportsInteriorColours() const noexcept
	{
		switch (model)
		{
		case ATSTRINGHASH_UNS("ardent"):
		case ATSTRINGHASH_UNS("asbo"):
		case ATSTRINGHASH_UNS("avisa"):
		case ATSTRINGHASH_UNS("baller3"):
		case ATSTRINGHASH_UNS("baller4"):
		case ATSTRINGHASH_UNS("baller5"):
		case ATSTRINGHASH_UNS("baller6"):
		case ATSTRINGHASH_UNS("baller7"):
		case ATSTRINGHASH_UNS("baller8"):
		case ATSTRINGHASH_UNS("brigham"):
		case ATSTRINGHASH_UNS("brioso3"):
		case ATSTRINGHASH_UNS("buccaneer2"):
		case ATSTRINGHASH_UNS("buffalo4"):
		case ATSTRINGHASH_UNS("buffalo5"):
		case ATSTRINGHASH_UNS("castigator"):
		case ATSTRINGHASH_UNS("champion"):
		case ATSTRINGHASH_UNS("cheburek"):
		case ATSTRINGHASH_UNS("cheetah2"):
		case ATSTRINGHASH_UNS("chino2"):
		case ATSTRINGHASH_UNS("clique"):
		case ATSTRINGHASH_UNS("clique2"):
		case ATSTRINGHASH_UNS("cog55"):
		case ATSTRINGHASH_UNS("cog552"):
		case ATSTRINGHASH_UNS("cognoscenti"):
		case ATSTRINGHASH_UNS("cognoscenti2"):
		case ATSTRINGHASH_UNS("comet6"):
		case ATSTRINGHASH_UNS("comet7"):
		case ATSTRINGHASH_UNS("coquette4"):
		case ATSTRINGHASH_UNS("coquette5"):
		case ATSTRINGHASH_UNS("corsita"):
		case ATSTRINGHASH_UNS("coureur"):
		case ATSTRINGHASH_UNS("cyclone"):
		case ATSTRINGHASH_UNS("cypher"):
		case ATSTRINGHASH_UNS("deity"):
		case ATSTRINGHASH_UNS("deluxo"):
		case ATSTRINGHASH_UNS("deveste"):
		case ATSTRINGHASH_UNS("dominator10"):
		case ATSTRINGHASH_UNS("dominator8"):
		case ATSTRINGHASH_UNS("dominator9"):
		case ATSTRINGHASH_UNS("driftcypher"):
		case ATSTRINGHASH_UNS("driftjester"):
		case ATSTRINGHASH_UNS("driftsentinel"):
		case ATSTRINGHASH_UNS("driftyosemite"):
		case ATSTRINGHASH_UNS("driftzr350"):
		case ATSTRINGHASH_UNS("ellie"):
		case ATSTRINGHASH_UNS("entity2"):
		case ATSTRINGHASH_UNS("entity3"):
		case ATSTRINGHASH_UNS("envisage"):
		case ATSTRINGHASH_UNS("eudora"):
		case ATSTRINGHASH_UNS("faction2"):
		case ATSTRINGHASH_UNS("faction3"):
		case ATSTRINGHASH_UNS("fagaloa"):
		case ATSTRINGHASH_UNS("gauntlet3"):
		case ATSTRINGHASH_UNS("gauntlet4"):
		case ATSTRINGHASH_UNS("gauntlet5"):
		case ATSTRINGHASH_UNS("gb200"):
		case ATSTRINGHASH_UNS("glendale2"):
		case ATSTRINGHASH_UNS("gp1"):
		case ATSTRINGHASH_UNS("growler"):
		case ATSTRINGHASH_UNS("ignus"):
		case ATSTRINGHASH_UNS("imorgon"):
		case ATSTRINGHASH_UNS("impaler"):
		case ATSTRINGHASH_UNS("impaler6"):
		case ATSTRINGHASH_UNS("infernus2"):
		case ATSTRINGHASH_UNS("issi3"):
		case ATSTRINGHASH_UNS("issi7"):
		case ATSTRINGHASH_UNS("italigto"):
		case ATSTRINGHASH_UNS("italirsx"):
		case ATSTRINGHASH_UNS("jester4"):
		case ATSTRINGHASH_UNS("jugular"):
		case ATSTRINGHASH_UNS("krieger"):
		case ATSTRINGHASH_UNS("l35"):
		case ATSTRINGHASH_UNS("le7b"):
		case ATSTRINGHASH_UNS("lm87"):
		case ATSTRINGHASH_UNS("manana2"):
		case ATSTRINGHASH_UNS("minivan2"):
		case ATSTRINGHASH_UNS("moonbeam2"):
		case ATSTRINGHASH_UNS("neo"):
		case ATSTRINGHASH_UNS("neon"):
		case ATSTRINGHASH_UNS("nero"):
		case ATSTRINGHASH_UNS("nero2"):
		case ATSTRINGHASH_UNS("niobe"):
		case ATSTRINGHASH_UNS("omnis"):
		case ATSTRINGHASH_UNS("paragon"):
		case ATSTRINGHASH_UNS("paragon2"):
		case ATSTRINGHASH_UNS("paragon3"):
		case ATSTRINGHASH_UNS("patriot3"):
		case ATSTRINGHASH_UNS("peyote3"):
		case ATSTRINGHASH_UNS("pipistrello"):
		case ATSTRINGHASH_UNS("poldominator10"):
		case ATSTRINGHASH_UNS("polgauntlet"):
		case ATSTRINGHASH_UNS("polimpaler6"):
		case ATSTRINGHASH_UNS("postlude"):
		case ATSTRINGHASH_UNS("primo2"):
		case ATSTRINGHASH_UNS("r300"):
		case ATSTRINGHASH_UNS("rapidgt3"):
		case ATSTRINGHASH_UNS("revolter"):
		case ATSTRINGHASH_UNS("rhinehart"):
		case ATSTRINGHASH_UNS("rt3000"):
		case ATSTRINGHASH_UNS("ruston"):
		case ATSTRINGHASH_UNS("sabregt2"):
		case ATSTRINGHASH_UNS("sc1"):
		case ATSTRINGHASH_UNS("schafter3"):
		case ATSTRINGHASH_UNS("schafter4"):
		case ATSTRINGHASH_UNS("schafter5"):
		case ATSTRINGHASH_UNS("schafter6"):
		case ATSTRINGHASH_UNS("schlagen"):
		case ATSTRINGHASH_UNS("sentinel3"):
		case ATSTRINGHASH_UNS("sentinel4"):
		case ATSTRINGHASH_UNS("slamvan3"):
		case ATSTRINGHASH_UNS("sm722"):
		case ATSTRINGHASH_UNS("specter"):
		case ATSTRINGHASH_UNS("specter2"):
		case ATSTRINGHASH_UNS("stafford"):
		case ATSTRINGHASH_UNS("stingertt"):
		case ATSTRINGHASH_UNS("stromberg"):
		case ATSTRINGHASH_UNS("sugoi"):
		case ATSTRINGHASH_UNS("swinger"):
		case ATSTRINGHASH_UNS("tahoma"):
		case ATSTRINGHASH_UNS("taipan"):
		case ATSTRINGHASH_UNS("tenf"):
		case ATSTRINGHASH_UNS("tenf2"):
		case ATSTRINGHASH_UNS("terminus"):
		case ATSTRINGHASH_UNS("tezeract"):
		case ATSTRINGHASH_UNS("thrax"):
		case ATSTRINGHASH_UNS("toreador"):
		case ATSTRINGHASH_UNS("torero"):
		case ATSTRINGHASH_UNS("torero2"):
		case ATSTRINGHASH_UNS("tornado5"):
		case ATSTRINGHASH_UNS("toros"):
		case ATSTRINGHASH_UNS("trophytruck"):
		case ATSTRINGHASH_UNS("trophytruck2"):
		case ATSTRINGHASH_UNS("tulip2"):
		case ATSTRINGHASH_UNS("turismo2"):
		case ATSTRINGHASH_UNS("turismo3"):
		case ATSTRINGHASH_UNS("tyrus"):
		case ATSTRINGHASH_UNS("vagner"):
		case ATSTRINGHASH_UNS("vectre"):
		case ATSTRINGHASH_UNS("vigero2"):
		case ATSTRINGHASH_UNS("vigero3"):
		case ATSTRINGHASH_UNS("virgo2"):
		case ATSTRINGHASH_UNS("virtue"):
		case ATSTRINGHASH_UNS("viseris"):
		case ATSTRINGHASH_UNS("visione"):
		case ATSTRINGHASH_UNS("voodoo"):
		case ATSTRINGHASH_UNS("weevil"):
		case ATSTRINGHASH_UNS("weevil2"):
		case ATSTRINGHASH_UNS("windsor2"):
		case ATSTRINGHASH_UNS("xa21"):
		case ATSTRINGHASH_UNS("yosemite"):
		case ATSTRINGHASH_UNS("yosemite1500"):
		case ATSTRINGHASH_UNS("yosemite2"):
		case ATSTRINGHASH_UNS("yosemite3"):
		case ATSTRINGHASH_UNS("youga3"):
		case ATSTRINGHASH_UNS("z190"):
		case ATSTRINGHASH_UNS("zeno"):
		case ATSTRINGHASH_UNS("zion3"):
		case ATSTRINGHASH_UNS("zr350"):
			return true;
		}

		return false;
	}
}
