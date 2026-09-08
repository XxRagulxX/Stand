#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

namespace Stand::Features
{
	// Real Stand's own CommandWaterWalk pokes a raw CPed struct field
	// (m_nPhysicalFlags.bIsNotBuoyant) this project has no equivalent
	// access to - reimplemented with a plain native technique instead:
	// while standing in water, hold you right at the water's own surface
	// height every tick rather than letting buoyancy/gravity pull you
	// under. Same end result (you walk on the surface), different
	// mechanism.
	class WaterWalk : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;

			if (!ENTITY::IS_ENTITY_IN_WATER(Self::GetPed().GetHandle()))
				return;

			const auto pos = Self::GetPed().GetPosition();

			float waterHeight = 0.0f;
			if (!WATER::GET_WATER_HEIGHT(pos.x, pos.y, pos.z, &waterHeight))
				return;

			if (pos.z < waterHeight)
				Self::GetPed().SetPosition({pos.x, pos.y, waterHeight});
		}
	};

	static WaterWalk _WaterWalk{"waterwalk", "Water Walk", "Walk on the surface of water"};
}
