#pragma once

#include "Commands/Widgets/CommandList.hpp"

#include "Config/Stats.hpp"

namespace Stand
{
	class CommandStatDate : public CommandList
	{
	private:
		Stat& stat;

	public:
		explicit CommandStatDate(CommandList* const parent, Stat& stat);
	};
}