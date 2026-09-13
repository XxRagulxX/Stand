#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandInfiniteParachutes : public CommandToggle
	{
	public:
		explicit CommandInfiniteParachutes(CommandList* const parent)
			: CommandToggle(parent, LIT("Infinite Parachutes"), CMDNAMES("infiniteparachutes"), LIT("Instantly replenishes your parachute."))
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
			PLAYER::SET_PLAYER_HAS_RESERVE_PARACHUTE(Stand::Self::GetPlayer().GetId());
			if (auto ped = Stand::Self::GetPed())
			{
				if (!ped.HasWeapon("GADGET_PARACHUTE"_J))
					ped.GiveWeapon("GADGET_PARACHUTE"_J);
			}
		}

		~CommandInfiniteParachutes() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
