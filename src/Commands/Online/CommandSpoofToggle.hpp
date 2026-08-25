#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandSpoofToggle : public CommandToggle
	{
	public:
		using CommandToggle::CommandToggle;

		void onChange(Click& click) final;
		void onEnable(Click& click) final;
	};
}
