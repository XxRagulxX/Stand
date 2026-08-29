#include "Commands/Self/CommandOutfitProp.hpp"

#include "Core/AbstractEntity.hpp"
#include "Commands/Self/CommandLockOutfit.hpp"
#include "Commands/Self/CommandOutfitPropVariation.hpp"
#include "Game/natives.hpp"

namespace Stand
{
	void CommandOutfitProp::onChange(Click& click, int prev_value)
	{
		ensureScriptThread(click, [this]
		{
			if (value == -1)
			{
				PED::CLEAR_PED_PROP(g_player_ped, data->id, 1);
			}
			else
			{
				PED::SET_PED_PROP_INDEX(g_player_ped, data->id, value, 0, true, 1);
			}
			CommandLockOutfit::ped_prop_commands[data->id].second->onDrawableChange();
		});
	}
}
