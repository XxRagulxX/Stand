#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"

namespace Stand::Features
{
	class InfiniteBoundary : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			PLAYER::EXTEND_WORLD_BOUNDARY_FOR_PLAYER(-1e5, -1e5, -1e5);
			PLAYER::EXTEND_WORLD_BOUNDARY_FOR_PLAYER(1e5, 1e5, 1e5);
		}
		virtual void OnDisable() override
			{
			PLAYER::RESET_WORLD_BOUNDARY_FOR_PLAYER();
		    }
	};

	static InfiniteBoundary Infinitebundary{"infiniteboundary", "Infinite Boundary", "You won't die because of border restrictions."};

}