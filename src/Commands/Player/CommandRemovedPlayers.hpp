#pragma once

#include "Commands/Widgets/CommandList.hpp"
#include "Network/HistoricPlayer.hpp"

namespace Stand
{
	class CommandRemovedPlayers : public CommandList
	{
	private:
		bool was_active = false;

	public:
		explicit CommandRemovedPlayers(CommandList* const parent);

		void onActiveListUpdate() final;
	};
}
