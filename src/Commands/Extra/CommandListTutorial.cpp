#include "Commands/Extra/CommandListTutorial.hpp"

#include "Commands/Extra/CommandRepeatTutorial.hpp"
#include "Commands/Extra/CommandDismissTutorials.hpp"

namespace Stand
{
	CommandListTutorial::CommandListTutorial(CommandList* const parent)
		: CommandList(parent, LOC("TUT"))
	{
		this->createChild<CommandRepeatTutorial>();
		this->createChild<CommandDismissTutorials>();
	}
}
