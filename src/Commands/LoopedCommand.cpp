#include "Commands/LoopedCommand.hpp"
#include "Commands/Commands.hpp"

namespace Stand
{
	LoopedCommand::LoopedCommand(std::string name, std::string label, std::string description) :
	    CommandToggleLegacy(name, label, description)
	{
		Commands::AddLoopedCommand(this);
	}

	void LoopedCommand::Tick()
	{
		OnTick();
	}
}