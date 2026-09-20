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
#include "Core/Pointers.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand
{
	void* ScriptGlobal::addr() const noexcept
	{
		return Pointers.ScriptGlobals[m_offset >> 0x12 & 0x3F] + (m_offset & 0x3FFFF);
	}

	bool ScriptGlobal::isAvailable() const noexcept
	{
		return Pointers.ScriptGlobals && (m_offset >> 0x12 & 0x3F) < 0x40 && Pointers.ScriptGlobals[m_offset >> 0x12 & 0x3F];
	}

	void ScriptGlobal::iterate(uint32_t elm_size, const std::function<void(uint32_t, ScriptGlobal&&)>& consumer) const
	{
		uint32_t count = get<uint32_t>();
		for (uint32_t i = 0; i < count; ++i)
			consumer(i, at(i, elm_size));
	}

	uint32_t ScriptGlobal::addrToOffset(void* addr)
	{
		for (uint32_t page = 0; page < 64; ++page)
		{
			auto* base = Pointers.ScriptGlobals[page];
			if (!base) continue;
			auto* ptr = reinterpret_cast<std::int64_t*>(addr);
			if (ptr >= base)
			{
				uint32_t within = static_cast<uint32_t>(ptr - base);
				if (within < 0x3FFFF)
					return (page << 18) | within;
			}
		}
		return 0;
	}
}

#define DEFINE_GLOBAL_ACCESSOR(str, global)         \
	str* str::Get()                                 \
	{                                               \
		auto glb = Stand::ScriptGlobal(global);     \
		if (!glb.isAvailable())                     \
			return nullptr;                         \
		return glb.as<str*>();                      \
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
