#pragma once

#include "Commands/Widgets/CommandSlider.hpp"

namespace Stand
{
	class CommandInfoScale : public CommandSlider
	{
	public:
		explicit CommandInfoScale(CommandList* const parent)
			: CommandSlider(parent, LOC("SCLE"), { CMDNAME("infoscale") }, NOLABEL, 0, 10000, 19)
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
