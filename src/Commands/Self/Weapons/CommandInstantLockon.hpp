#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
	class CommandInstantLockon : public CommandToggle
	{
	public:
		explicit CommandInstantLockon(CommandList* const parent)
			: CommandToggle(parent, LIT("Instant Lock-On"), CMDNAMES("instantlockon"), NOLABEL)
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			for (Vehicle veh : Pools::GetVehicles())
			{
				const int h = veh.GetHandle();
				if (VEHICLE::GET_VEHICLE_HOMING_LOCKEDONTO_STATE(h) == 1)
					VEHICLE::SET_VEHICLE_HOMING_LOCKEDONTO_STATE(h, 2);
			}
		}
	};
}
