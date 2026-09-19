#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandInfiniteAmmo : public CommandToggle
	{
	public:
		explicit CommandInfiniteAmmo(CommandList* const parent)
			: CommandToggle(parent, LIT("Infinite Ammo"), CMDNAMES("infiniteammo"), LIT("Never run out of ammo."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (auto ped = Stand::Self::GetPed())
				ped.SetInfiniteAmmo(false);
		}

		void onTick() override
		{
			if (auto ped = Stand::Self::GetPed())
				ped.SetInfiniteAmmo(true);
		}

		~CommandInfiniteAmmo() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
