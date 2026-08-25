#include "Commands/Online/CommandMmRegion.hpp"

#include "Commands/Online/CommandMmRegionToggle.hpp"
#include "Commands/Online/CommandMmRegionSelect.hpp"

namespace Stand
{
	CommandMmRegion::CommandMmRegion(CommandList* const parent)
		: CommandList(parent, LOC("MMREGION2"))
	{
		toggle = createChild<CommandMmRegionToggle>();
		select = createChild<CommandMmRegionSelect>();
	}
}
