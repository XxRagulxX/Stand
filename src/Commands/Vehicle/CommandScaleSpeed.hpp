#pragma once

#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
	class CommandScaleSpeed : public CommandToggle
	{
	public:
		explicit CommandScaleSpeed(CommandList* const parent)
			: CommandToggle(parent, LOC("SCLESPD"), { CMDNAME("scalespeed") }, NOLABEL, true)
		{
		}
	};
}
