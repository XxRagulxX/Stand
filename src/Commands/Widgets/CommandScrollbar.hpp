#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"

namespace Stand
{
	class CommandScrollbar : public CommandListSelect
	{
	public:
		explicit CommandScrollbar(CommandList* parent);
		
		void onChange(Click& click, long long prev_value) final;
	};
}
