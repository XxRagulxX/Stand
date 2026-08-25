#include "Commands/Online/CommandListSessionSpoof.hpp"

#include "Commands/Online/CommandHideSession.hpp"
#include "Commands/Online/CommandSpoofSessionType.hpp"
#include "Commands/Online/CommandSpoofHost.hpp"
#include "Commands/Online/CommandSpoofedHost.hpp"

namespace Stand
{
	CommandListSessionSpoof::CommandListSessionSpoof(CommandList* parent)
		: CommandList(parent, LOC("SPFSESS"))
	{
		this->createChild<CommandHideSession>();
		this->createChild<CommandSpoofSessionType>();
		{
			auto host = this->createChild<CommandList>(LOC("FLAG_HOST"));

			host->createChild<CommandSpoofHost>();
			host->createChild<CommandSpoofedHost>();
		}
	}
}
