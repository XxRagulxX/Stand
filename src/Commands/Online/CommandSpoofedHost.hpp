#pragma once

#include "Commands/Online/CommandGamer.hpp"

#include "Commands/Widgets/CommandLambdaAction.hpp"
#include "Game/pointers.hpp"
#include "Network/rlGamerInfo.hpp"
#include "Network/SessionSpoofing.hpp"

namespace Stand
{
	class CommandSpoofedHost : public CommandGamer
	{
	public:
		inline static CommandSpoofedHost* instance;

		explicit CommandSpoofedHost(CommandList* parent)
			: CommandGamer(parent, LANG_GET("SPFDHST"), CMDNAMES("spoofedhost"), false)
		{
			this->createChild<CommandLambdaAction>(LOC("YOU"), {}, NOLABEL, [this](Click& click)
			{
				this->onPresetClick(click, pointers::rlPresence_GamerPresences_0_GamerInfo->getHandle().rockstar_id, pointers::rlPresence_GamerPresences_0_GamerInfo->name);
			});

			instance = this;
		}
	};
}
