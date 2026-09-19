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
	class CommandNoSpooling : public CommandToggle
	{
	public:
		explicit CommandNoSpooling(CommandList* const parent)
			: CommandToggle(parent, LIT("No Spooling"), CMDNAMES("nospooling"),
			                LIT("Removes the delay before firing from applicable weapons."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			AUDIO::SET_SKIP_MINIGUN_SPIN_UP_AUDIO(FALSE);
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;

			Hash weapon{};
			WEAPON::GET_CURRENT_PED_WEAPON(ped.GetHandle(), &weapon, FALSE);

			if (weapon == "WEAPON_MINIGUN"_J || weapon == "WEAPON_RAYMINIGUN"_J)
			{
				AUDIO::SET_SKIP_MINIGUN_SPIN_UP_AUDIO(TRUE);
				PAD::SET_CONTROL_VALUE_NEXT_FRAME(0, 24, 1.0f);
			}
			else
			{
				AUDIO::SET_SKIP_MINIGUN_SPIN_UP_AUDIO(FALSE);
			}
		}

		~CommandNoSpooling() override
		{
			CommandTickDispatch::RemoveCommand(this);
			AUDIO::SET_SKIP_MINIGUN_SPIN_UP_AUDIO(FALSE);
		}
	};
}
