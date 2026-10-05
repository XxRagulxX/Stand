#include "Commands/Widgets/Command.hpp"
#include "Commands/Widgets/CommandListSelect.hpp"

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

	bool Command::shouldShowUntrimmedName() const
	{
		if (type == COMMAND_LIST_SELECT)
			return as<CommandListSelect>()->getCurrentValueHelpText().empty();
		return true;
	}
}
