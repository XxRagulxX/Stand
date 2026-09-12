#pragma once
#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandHairHighlight : public CommandSlider
	{
	public:
		explicit CommandHairHighlight(CommandList* parent)
			: CommandSlider(parent, LIT("Hair Highlight"), CMDNAMES("hairhighlight", "hairlight"), NOLABEL, -1, 64, -1, 1, 0)
		{
		}

		void onChange(Click& click, int prev_value) final;
		void applyDefaultState() final {}
	};
}
