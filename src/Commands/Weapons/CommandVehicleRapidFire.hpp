#pragma once
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Menu/GUI.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Util/Math.hpp"
#include "World/Self.hpp"

#include <cstddef>

namespace Stand
{
	class CommandVehicleRapidFire : public CommandToggle
	{
		const std::size_t m_TickRate    = 5u;
		std::size_t       m_CurrentTick = 0;

		bool canShoot()
		{
			bool result   = (m_CurrentTick == 0);
			m_CurrentTick = (m_CurrentTick + 1) % m_TickRate;
			return result;
		}

	public:
		explicit CommandVehicleRapidFire(CommandList* const parent)
			: CommandToggle(parent, LIT("Rapid Fire Vehicle Weapons"), CMDNAMES("vehiclerapidfire"),
			                LIT("Rapidly fires your current vehicle weapon."))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			m_CurrentTick = 0;
		}

		void onTick() override
		{
			if (HUD::IS_PAUSE_MENU_ACTIVE() || GUI::IsOpen())
				return;

			auto ped = Stand::Self::GetPed();
			if (!ped || ped.IsDead())
				return;

			const int pedHandle = ped.GetHandle();

			if (!PED::IS_PED_IN_ANY_VEHICLE(pedHandle, false))
				return;

			if (!PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_VEH_ATTACK))
				return;

			if (!canShoot())
				return;

			Hash vehicleWeapon{};
			if (!WEAPON::GET_CURRENT_PED_VEHICLE_WEAPON(pedHandle, &vehicleWeapon))
				return;

			const int vehHandle = PED::GET_VEHICLE_PED_IS_IN(pedHandle, false);

			auto camDir = Math::RotationToDirection(CAMERA::GET_GAMEPLAY_CAM_ROT(0));
			auto camPos = CAMERA::GET_GAMEPLAY_CAM_COORD() + camDir;
			Vector3 end = camPos + camDir * 2000.0f;

			const auto raycast = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
				camPos.x, camPos.y, camPos.z,
				end.x, end.y, end.z,
				-1, vehHandle, 7);

			BOOL    didHit{FALSE};
			Vector3 hitPos{};
			Vector3 norm{};
			int     hitEnt{};
			SHAPETEST::GET_SHAPE_TEST_RESULT(raycast, &didHit, &hitPos, &norm, &hitEnt);
			if (didHit)
				end = hitPos;

			const Vector3 startPos = ENTITY::GET_ENTITY_COORDS(vehHandle, true);

			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(
				startPos.x, startPos.y, startPos.z,
				end.x, end.y, end.z,
				static_cast<int>(WEAPON::GET_WEAPON_DAMAGE(vehicleWeapon, 0)),
				true,
				vehicleWeapon,
				pedHandle,
				true, false, -1.0f);
		}

		~CommandVehicleRapidFire() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
