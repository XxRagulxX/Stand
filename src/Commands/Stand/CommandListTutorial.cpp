#include "Commands/Stand/CommandListTutorial.hpp"

namespace Stand
{
	CommandListTutorial::CommandListTutorial(CommandList* const parent)
		: CommandList(parent, LOC("TUT"))
	{
	}
}
