#include "Commands/Stand/CommandListMental.hpp"

#include "Commands/Self/CommandMental.hpp"
#include "Commands/Stand/CommandLockMentalState.hpp"
#include "Commands/Self/CommandMentalPulse.hpp"

namespace Stand
{
	CommandListMental::CommandListMental(CommandList* const parent)
		: CommandList(parent, LOC("MNTLS"))
	{
		mental = this->createChild<CommandMental>();
		this->createChild<CommandLockMentalState>();
		pulse = this->createChild<CommandMentalPulse>();
	}
}
