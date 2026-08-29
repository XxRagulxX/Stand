#pragma once

#include "Commands/Player/CommandPlayerToggleBatch.hpp"

namespace Stand
{
	class CommandPlayerNotifySpam : public CommandPlayerToggleBatch
	{
	public:
		explicit CommandPlayerNotifySpam(CommandList* const parent);

		void onTick(const std::vector<AbstractPlayer>& players) const final;
	};
}
