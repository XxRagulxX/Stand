#pragma once
#include "Commands/Widgets/CommandSlider.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandWanted : public CommandSlider
	{
	public:
		explicit CommandWanted(CommandList* const parent)
			: CommandSlider(parent, LIT("Set Wanted Level"), { CMDNAME("wanted"), CMDNAME("wantedlevel"), CMDNAME("wantedslider") }, NOLABEL, 0, 5, 0)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			Stand::Self::GetPlayer().SetWantedLevel(value);
		}
	};
}
