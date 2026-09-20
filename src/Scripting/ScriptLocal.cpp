#include "World/FMRandomEvents.hpp"
#include "Vehicle/VehicleRewardData.hpp"
#include "Vehicle/StandardTimeTrialData.hpp"
#include "Vehicle/RCBanditoTimeTrialData.hpp"
#include "Vehicle/HSWTimeTrialData.hpp"
#include "Vehicle/BikeTimeTrialData.hpp"
#include "Network/LauncherClientData.hpp"
#include "Scripting/ScriptLocal.hpp"
#include "Scripting/Scripts.hpp"

namespace Stand
{
	ScriptLocal::ScriptLocal(rage::scrThread* thread, std::size_t index) :
	    m_stack(thread->m_Stack),
	    m_index(index)
	{
	}

	ScriptLocal::ScriptLocal(void* stack, std::size_t index) :
	    m_stack(stack),
	    m_index(index)
	{
	}

ScriptLocal ScriptLocal::at(std::ptrdiff_t index)
	{
		return ScriptLocal(m_stack, m_index + index);
	}

	ScriptLocal ScriptLocal::at(std::ptrdiff_t index, std::size_t size)
	{
		return ScriptLocal(m_stack, m_index + 1 + index * size);
	}

	void* ScriptLocal::get()
	{
		return reinterpret_cast<uintptr_t*>((uintptr_t)m_stack + (m_index * sizeof(uintptr_t)));
	}
}

#define DEFINE_LOCAL_ACCESSOR(str, local)                    \
	str* str::Get(rage::scrThread* thread)                   \
	{                                                        \
		if (!thread) return nullptr;                         \
		return Stand::ScriptLocal(thread, local).as<str*>(); \
	}

DEFINE_LOCAL_ACCESSOR(VEHICLE_REWARD_DATA, 148);
DEFINE_LOCAL_ACCESSOR(RANDOM_EVENTS_FREEMODE_DATA, 16199);
DEFINE_LOCAL_ACCESSOR(StandardTimeTrialData, 14774);
DEFINE_LOCAL_ACCESSOR(RCBanditoTimeTrialData, 14824);
DEFINE_LOCAL_ACCESSOR(HSWTimeTrialData, 14868);
DEFINE_LOCAL_ACCESSOR(BikeTimeTrialData, 15667);
DEFINE_LOCAL_ACCESSOR(LauncherClientData, 270);
