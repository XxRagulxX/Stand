#include "Commands/Self/CommandWetness.hpp"

#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	void CommandWetness::onChange(Click& click, int prev_value)
	{
		ensureScriptThread(click, [this]
		{
			const int ped = Self::GetPed().GetHandle();
			if (value == 0)
				PED::CLEAR_PED_WETNESS(ped);
			else
				PED::SET_PED_WETNESS_HEIGHT(ped, (((float)value * 0.2f) - 110.0f) / 100.0f);
		});
	}
}
