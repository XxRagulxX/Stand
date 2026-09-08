#pragma once
#include "Commands/CommandToggleLegacy.hpp"

namespace Stand
{
	class LoopedCommand : public CommandToggleLegacy
	{
	protected:
		virtual void OnTick() = 0;

	public:
		LoopedCommand(std::string name, std::string label, std::string description);
		void Tick();
	};
}