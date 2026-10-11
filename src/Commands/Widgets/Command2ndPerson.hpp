#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

#include "Rendering/Gui.hpp"

namespace Stand
{
	class Command2ndPerson : public CommandToggle
	{
	public:
		explicit Command2ndPerson(CommandList* const parent)
			: CommandToggle(parent, LOC("2PRSN"), CMDNAMES("2ndperson", "secondperson"))
		{
		}

		void onEnable(Click& click) final
		{
		}
	};
}
