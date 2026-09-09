#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandRefillHealth : public CommandPhysical
	{
	public:
		explicit CommandRefillHealth(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Refill Health"), CMDNAMES("refillhealth"), LIT("Restores your health to maximum."))
		{
		}

		void onClick(Click& click) override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;
			ped.SetHealth(ped.GetMaxHealth());
		}
	};
}
