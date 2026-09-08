#include "Commands/Widgets/Command.hpp"

namespace Stand
{
	CommandPhysical* Command::getPhysical() noexcept
	{
		return isPhysical() ? as<CommandPhysical>() : nullptr;
	}

	const CommandPhysical* Command::getPhysical() const noexcept
	{
		return isPhysical() ? as<CommandPhysical>() : nullptr;
	}
}
