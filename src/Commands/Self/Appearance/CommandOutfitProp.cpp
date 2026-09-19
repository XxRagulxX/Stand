#include "Commands/Self/Appearance/CommandOutfitProp.hpp"

#include "Commands/Self/Appearance/CommandLockOutfit.hpp"
#include "Commands/Self/Appearance/CommandOutfitPropVariation.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand
{
	void CommandOutfitProp::onChange(Click& click, int prev_value)
	{
		ensureScriptThread(click, [this]
		{
			const int ped = Self::GetPed().GetHandle();
			if (value == -1)
				PED::CLEAR_PED_PROP(ped, data->id, 1);
			else
				PED::SET_PED_PROP_INDEX(ped, data->id, value, 0, TRUE, 1);
			CommandLockOutfit::ped_prop_commands[data->id].second->onDrawableChange();
		});
	}
}
