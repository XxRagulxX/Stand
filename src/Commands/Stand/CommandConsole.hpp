#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/LogHelper.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	class CommandConsole : public CommandToggle
	{
	public:
		explicit CommandConsole(CommandList* parent) :
		    CommandToggle(parent, LIT("Console"), CMDNAMES("console"), LIT("Shows or hides the debug console window."))
		{
		}

		void onEnable(Click& click) override
		{
			LogHelper::ToggleConsole(true);
		}

		void onDisable(Click& click) override
		{
			LogHelper::ToggleConsole(false);
		}
	};
}
