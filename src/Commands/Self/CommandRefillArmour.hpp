#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandRefillArmour : public CommandPhysical
	{
	public:
		explicit CommandRefillArmour(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Refill Armour"), CMDNAMES("refillarmour"), LIT("Restores your armour to maximum."))
		{
		}

		void onClick(Click& click) override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;
			ped.SetArmour(Stand::Self::GetPlayer().GetMaxArmour());
		}
	};
}
