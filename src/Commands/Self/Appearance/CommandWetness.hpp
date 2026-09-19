#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandWetness final : public CommandSlider
	{
	public:
		explicit CommandWetness(CommandList* parent)
			: CommandSlider(parent, LIT("Set Wetness"), CMDNAMES("wetness"), NOLABEL, 0, 1000, 0, 5, 0)
		{
		}

		void onChange(Click& click, int prev_value) final;
		void applyDefaultState() final {}
	};
}
