#pragma once

#include "Commands/Widgets/CommandList.hpp"

namespace Stand
{
	class CommandCustomGlider : public CommandList
	{
	public:
		CommandSlider* min;
		CommandSlider* max;
		CommandSlider* speed;

		explicit CommandCustomGlider(CommandList* const parent);
	};
}
