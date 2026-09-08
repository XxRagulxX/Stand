#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Ported from real Stand's own CommandAirwalk: no gravity while
	// airborne (walk on thin air), gravity restored the instant you
	// ragdoll (falling still hurts if something knocks you down) or turn
	// this off. Simplified from Stand's own version in one place: real
	// Stand also checks `!GET_IS_TASK_ACTIVE(ped, CTaskTypes::TASK_JUMP)`
	// before re-grounding on landing, using a task-type constant this
	// project has no verified numeric value for - left out rather than
	// guess at an offset that could silently misbehave; the only cost is
	// a jump's own initial upward velocity occasionally getting zeroed a
	// frame early on landing, not a safety concern.
	class Airwalk : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			const auto handle = Self::GetPed().GetHandle();

			if (PED::IS_PED_RAGDOLL(handle))
			{
				ENTITY::SET_ENTITY_HAS_GRAVITY(handle, true);
				PED::SET_PED_GRAVITY(handle, true);
			}
			else
			{
				ENTITY::SET_ENTITY_HAS_GRAVITY(handle, false);
				PED::SET_PED_GRAVITY(handle, false);
			}

			if (!ENTITY::IS_ENTITY_IN_AIR(handle))
			{
				const auto vel = ENTITY::GET_ENTITY_VELOCITY(handle);
				if (vel.z != 0.0f)
					ENTITY::SET_ENTITY_VELOCITY(handle, vel.x, vel.y, 0.0f);
			}
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			ENTITY::SET_ENTITY_HAS_GRAVITY(Self::GetPed().GetHandle(), true);
			PED::SET_PED_GRAVITY(Self::GetPed().GetHandle(), true);
		}
	};

	static Airwalk _Airwalk{"airwalk", "Airwalk", "Walk on thin air"};
}
