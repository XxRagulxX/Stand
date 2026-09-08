#include "World/FMRandomEvents.hpp"
#include "Vehicle/VehicleRewardData.hpp"
#include "Vehicle/StandardTimeTrialData.hpp"
#include "Vehicle/RCBanditoTimeTrialData.hpp"
#include "Vehicle/HSWTimeTrialData.hpp"
#include "Vehicle/BikeTimeTrialData.hpp"
#include "Network/LauncherClientData.hpp"
#include "Scripting/ScriptLocal.hpp"

#define DEFINE_LOCAL_ACCESSOR(str, local)               \
	str* str::Get(rage::scrThread* thread)              \
	{                                                   \
		auto lcl = Stand::ScriptLocal(thread, local); \
		if (!lcl.CanAccess())                           \
			return nullptr;                             \
		return lcl.As<str*>();                          \
	}

DEFINE_LOCAL_ACCESSOR(VEHICLE_REWARD_DATA, 148);
DEFINE_LOCAL_ACCESSOR(RANDOM_EVENTS_FREEMODE_DATA, 16199);
DEFINE_LOCAL_ACCESSOR(StandardTimeTrialData, 14774);
DEFINE_LOCAL_ACCESSOR(RCBanditoTimeTrialData, 14824);
DEFINE_LOCAL_ACCESSOR(HSWTimeTrialData, 14868);
DEFINE_LOCAL_ACCESSOR(BikeTimeTrialData, 15667);

// am_launcher
DEFINE_LOCAL_ACCESSOR(LauncherClientData, 270);