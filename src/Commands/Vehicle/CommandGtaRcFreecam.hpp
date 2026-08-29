#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandGtaRcFreecam : public CommandToggle
	{
	public:
		explicit CommandGtaRcFreecam(CommandList* const parent);

		void onChange(Click& click) final;
	};
}
