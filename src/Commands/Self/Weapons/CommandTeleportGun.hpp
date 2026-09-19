#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Util/Math.hpp"
#include "World/Self.hpp"

namespace Stand
{
	class CommandTeleportGun : public CommandToggle
	{
		bool m_wasShooting = false;

	public:
		explicit CommandTeleportGun(CommandList* const parent)
			: CommandToggle(parent, LIT("Teleport Gun"), CMDNAMES("tpgun"), NOLABEL)
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			m_wasShooting = false;
		}

		void onTick() override
		{
			auto ped = Stand::Self::GetPed();
			if (!ped || ped.IsDead())
			{
				m_wasShooting = false;
				return;
			}

			const int pedHandle = ped.GetHandle();
			const bool isShooting = PED::IS_PED_SHOOTING(pedHandle) == TRUE;
			const bool risingEdge = isShooting && !m_wasShooting;
			m_wasShooting = isShooting;

			if (!risingEdge)
				return;

			auto camDir = Math::RotationToDirection(CAMERA::GET_GAMEPLAY_CAM_ROT(0));
			auto camPos = CAMERA::GET_GAMEPLAY_CAM_COORD();
			Vector3 end = camPos + camDir * 2000.0f;

			const auto ray = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
				camPos.x, camPos.y, camPos.z,
				end.x, end.y, end.z,
				-1, 0, 7);

			BOOL didHit{FALSE};
			Vector3 hitPos{};
			Vector3 norm{};
			int hitEnt{};
			SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &didHit, &hitPos, &norm, &hitEnt);

			if (!didHit)
				return;

			const Vector3 dest{hitPos.x, hitPos.y, hitPos.z + 1.0f};
			const float heading = ENTITY::GET_ENTITY_HEADING(pedHandle);

			int handle = pedHandle;
			if (auto veh = Stand::Self::GetVehicle())
				handle = veh.GetHandle();

			FiberPool::queueJob([dest, heading, handle] {
				STREAMING::REQUEST_COLLISION_AT_COORD(dest.x, dest.y, dest.z);
				int timeout = 0;
				while (!ENTITY::HAS_COLLISION_LOADED_AROUND_ENTITY(handle) && timeout < 50)
				{
					BUILTIN::WAIT(10);
					timeout++;
				}
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(handle, dest.x, dest.y, dest.z, false, false, false);
				ENTITY::SET_ENTITY_HEADING(handle, heading);
			});
		}

		~CommandTeleportGun() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
