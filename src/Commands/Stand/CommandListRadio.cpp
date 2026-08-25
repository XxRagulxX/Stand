#include "Commands/Stand/CommandListRadio.hpp"

#include "Commands/Stand/CommandListRadioStations.hpp"
#include "Commands/World/CommandMobileRadio.hpp"
#include "Commands/Vehicle/CommandLockRadio.hpp"

namespace Stand
{
	CommandListRadio::CommandListRadio(CommandList* const parent)
		: CommandList(parent, LOC("RD"))
	{
		this->createChild<CommandListRadioStations>();
		this->createChild<CommandMobileRadio>();
		this->createChild<CommandLockRadio>();
	}
}
