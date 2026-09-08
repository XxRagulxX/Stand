#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandNoCollision (its own menu name
	// is "Ghost Ride" - separate from Movement's own Ghost/Reduced
	// Collision, see CommandGhostMode.cpp, which shrinks the collision
	// capsule rather than disabling collision outright).
	class NoCollision : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(Self::GetPed().GetHandle(), false, false);
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			ENTITY::SET_ENTITY_COLLISION(Self::GetPed().GetHandle(), true, false);
		}
	};

	static NoCollision _NoCollision{"nocollision", "No Collision", "Pass through everything"};
}
