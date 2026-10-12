#pragma once

#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandInfoDropShadow : public CommandSlider
	{
	public:
		explicit CommandInfoDropShadow(CommandList* const parent)
			: CommandSlider(parent, LOC("DRPSHDW"), { CMDNAME("infodropshadow") }, NOLABEL, 0, 255, 1)
		{
		}

		void onFocus(ThreadContext thread_context, Direction momentum) final
		{
		}

		void onBlur(ThreadContext thread_context, Direction momentum) final
		{
		}

		void onChange(Click& click, int prev_value) final
		{
		}
	};
}
