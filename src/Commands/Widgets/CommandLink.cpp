#include "Commands/Widgets/CommandLink.hpp"

#include <soup/WeakRef.hpp>

#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
	CommandLink::CommandLink(CommandList* parent, Command* target, bool show_address_in_corner)
		: CommandLink(parent, (CommandPhysical*)target, show_address_in_corner)
	{
	}

	CommandLink::CommandLink(CommandList* parent, CommandPhysical* target, bool show_address_in_corner)
		: Command(COMMAND_LINK, parent), target(target->getWeakRef()), show_address_in_corner(show_address_in_corner)
	{
	}

	CommandPhysical* CommandLink::getTarget() const noexcept
	{
		return target.getPointer()->as<CommandPhysical>();
	}
}
