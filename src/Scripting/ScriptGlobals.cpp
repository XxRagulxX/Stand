#include "Network/GPBD_FM.hpp"
#include "Network/GPBD_FM_2.hpp"
#include "Network/GPBD_FM_3.hpp"
#include "Network/GPBD_Flow.hpp"
#include "Network/GPBD_Kicking.hpp"
#include "Network/GPBD_MissionName.hpp"
#include "Network/GSBD.hpp"
#include "Network/GSBD_BlockB.hpp"
#include "Network/GSBD_FM.hpp"
#include "Network/GSBD_FM_Events.hpp"
#include "Network/GSBD_Kicking.hpp"
#include "Network/GSBD_PropertyInstances.hpp"
#include "Network/GSBD_RandomEvents.hpp"
#include "Network/GlobalPlayerBD.hpp"
#include "Network/g_AMC_playerBD.hpp"
#include "Network/MPSV.hpp"
#include "Network/g_SavedMPGlobals.hpp"
#include "Network/FreemodeGeneral.hpp"
#include "World/DailyObjectivesState.hpp"
#include "World/WeeklyChallenge.hpp"
#include "Network/FreemodeDelivery.hpp"
#include "Network/LauncherServerData.hpp"
#include "Scripting/ScriptGlobal.hpp"

#define DEFINE_GLOBAL_ACCESSOR(str, global)       \
	str* str::Get()                               \
	{                                             \
		auto glb = Stand::ScriptGlobal(global); \
		if (!glb.CanAccess())                     \
			return nullptr;                       \
		return glb.As<str*>();                    \
	}

DEFINE_GLOBAL_ACCESSOR(GlobalPlayerBD, 2658296);
DEFINE_GLOBAL_ACCESSOR(GPBD_FM, 1845347);
DEFINE_GLOBAL_ACCESSOR(GPBD_FM_2, 1882797);
DEFINE_GLOBAL_ACCESSOR(GPBD_FM_3, 1893070);
DEFINE_GLOBAL_ACCESSOR(GPBD_Flow, 1985024);
DEFINE_GLOBAL_ACCESSOR(GPBD_Kicking, 1874149);
DEFINE_GLOBAL_ACCESSOR(GPBD_MissionName, 1873636);
DEFINE_GLOBAL_ACCESSOR(GSBD, 2649164);
DEFINE_GLOBAL_ACCESSOR(GSBD_BlockB, 2650428);
DEFINE_GLOBAL_ACCESSOR(GSBD_FM, 1835603);
DEFINE_GLOBAL_ACCESSOR(GSBD_FM_Events, 1923118);
DEFINE_GLOBAL_ACCESSOR(GSBD_Kicking, 1877350);
DEFINE_GLOBAL_ACCESSOR(GSBD_PropertyInstances, 1944313);
DEFINE_GLOBAL_ACCESSOR(g_AMC_playerBD, 2700299);
DEFINE_GLOBAL_ACCESSOR(GSBD_RandomEvents, 1882345);
DEFINE_GLOBAL_ACCESSOR(MPSV, 1583778);
DEFINE_GLOBAL_ACCESSOR(g_SavedMPGlobals, 2359296);
DEFINE_GLOBAL_ACCESSOR(FreemodeGeneral, 2733326);
DEFINE_GLOBAL_ACCESSOR(DailyObjectivesState, 1574746);
DEFINE_GLOBAL_ACCESSOR(WeeklyChallenge, 2732247);
DEFINE_GLOBAL_ACCESSOR(FreemodeDelivery, 1957909);
DEFINE_GLOBAL_ACCESSOR(LauncherServerData, 2700113);