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
	class CommandNoReload : public CommandToggle
	{
	public:
		explicit CommandNoReload(CommandList* const parent)
			: CommandToggle(parent, LIT("Skip Reloading"), CMDNAMES("noreload", "skipreloading"), LIT("Skips the reload animation by instantly refilling your clip."))
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
			if (auto ped = Stand::Self::GetPed())
			{
				Hash weapon{};
				WEAPON::GET_CURRENT_PED_WEAPON(ped.GetHandle(), &weapon, FALSE);
				if (weapon != "WEAPON_MINIGUN"_J && weapon != "WEAPON_RAYMINIGUN"_J)
					WEAPON::REFILL_AMMO_INSTANTLY(ped.GetHandle());
			}
		}

		~CommandNoReload() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
