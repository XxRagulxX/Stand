#pragma once

#include "Commands/Widgets/CommandListSelect.hpp"

namespace Stand
{
	class CommandWaterWaveBehaviour : public CommandListSelect
	{
	public:
		explicit CommandWaterWaveBehaviour(CommandList* const parent);

		void onChange(Click& click, long long prev_value) final;
	};
}
