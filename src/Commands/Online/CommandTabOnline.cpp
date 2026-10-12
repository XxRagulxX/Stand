#include "Commands/Online/CommandTabOnline.hpp"

namespace Stand
{
	CommandTabOnline::CommandTabOnline(CommandList* const parent)
		: CommandTab(parent, TAB_ONLINE, LOC("Online"), { CMDNAME("tonline"), CMDNAME("online") })
	{
	}

	void CommandTabOnline::populate()
	{
	}
}
