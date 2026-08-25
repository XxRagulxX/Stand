#pragma once

#include "Commands/Widgets/CommandList.hpp"

#include "Config/Stats.hpp"

namespace Stand
{
	class CommandStatPos : public CommandList
	{
	private:
		Stat& stat;

	public:
		explicit CommandStatPos(CommandList* const parent, Stat& stat);
	};
}