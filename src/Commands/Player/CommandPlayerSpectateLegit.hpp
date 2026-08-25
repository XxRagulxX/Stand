#pragma once

#include "Commands/Player/CommandPlayerSpectate.hpp"

namespace Stand
{
	class CommandPlayerSpectateLegit : public CommandPlayerSpectate
	{
	public:
		explicit CommandPlayerSpectateLegit(CommandList* const parent);

		void onEnable(Click& click) final;
	};
}
