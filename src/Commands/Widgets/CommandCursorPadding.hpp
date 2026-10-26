#pragma once

#include "Commands/Widgets/CommandSlider.hpp"
#include "Rendering/Theme.hpp"

namespace Stand
{
	class CommandCursorPadding : public CommandSlider
	{
	public:
		explicit CommandCursorPadding(CommandList* const parent)
			: CommandSlider(parent, LOC("SCRLGP"), { CMDNAME("cursorpadding") }, LOC("CRSPAD_H"), 0, 25, 2, 1)
		{
		}

		void onChange(Click& click, int prev_value) final
		{
			Rendering::Theme::kCursorPadding = value;
		}
	};
}
