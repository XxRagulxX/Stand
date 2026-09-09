#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Menu/Click.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandSuicide : public CommandPhysical
	{
	public:
		explicit CommandSuicide(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Suicide"), CMDNAMES("suicide"), LIT("Kills you."))
		{
		}

		void onClick(Click& click) override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped)
				return;
			ped.SetInvincible(false);
			ped.SetHealth(0);
		}
	};
}
