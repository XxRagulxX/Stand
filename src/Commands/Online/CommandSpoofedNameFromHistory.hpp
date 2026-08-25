#pragma once

#include "Commands/Widgets/CommandAction.hpp"

#include "Network/PlayerHistory.hpp"

namespace Stand
{
	class CommandSpoofedNameFromHistory : public CommandAction
	{
	public:
		explicit CommandSpoofedNameFromHistory(CommandList* const parent)
			: CommandAction(parent, LOC("SPFNMEFRMHST"))
		{
		}

		void onClick(Click& click) final
		{
			auto* entry = PlayerHistory::getRandomEntry();
			if (entry != nullptr)
			{
				CommandSpoofedName::instance->setName(click, entry->name);
			}
		}
	};
}
