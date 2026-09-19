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
	class CommandRapidFire : public CommandToggle
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
		explicit CommandRapidFire(CommandList* const parent)
			: CommandToggle(parent, LIT("Rapid Fire"), CMDNAMES("rapidfire"), LIT("Rapidly fires your current weapon."))
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

			if (!PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_ATTACK) || !canShoot())
				return;

			const auto weapon_entity = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(ped.GetHandle(), 0);
			if (!weapon_entity)
				return;

			Vector3 dim_min{};
			Vector3 dim_max{};
			MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(weapon_entity), &dim_min, &dim_max);

			const auto dimensions      = (dim_max - dim_min) * 0.5f;
			const auto weapon_position = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(
				weapon_entity, dimensions.x, 0.0f, dimensions.z * 0.3f);

			auto camera_direction = Math::RotationToDirection(CAMERA::GET_GAMEPLAY_CAM_ROT(0));
			auto camera_position  = CAMERA::GET_GAMEPLAY_CAM_COORD() + camera_direction;
			Vector3 end           = camera_position + camera_direction * 2000.0f;

			const auto raycast_handle = SHAPETEST::START_EXPENSIVE_SYNCHRONOUS_SHAPE_TEST_LOS_PROBE(
				camera_position.x, camera_position.y, camera_position.z,
				end.x, end.y, end.z,
				-1, 0, 7);

			BOOL    did_hit{FALSE};
			Vector3 hit_pos{};
			Vector3 surface_normal{};
			int     hit_entity{};
			SHAPETEST::GET_SHAPE_TEST_RESULT(raycast_handle, &did_hit, &hit_pos, &surface_normal, &hit_entity);
			if (did_hit)
				end = hit_pos;

			const Hash weapon_hash = ped.GetCurrentWeapon();

			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(
				weapon_position.x, weapon_position.y, weapon_position.z,
				end.x, end.y, end.z,
				WEAPON::GET_WEAPON_DAMAGE(weapon_hash, 0),
				true,
				weapon_hash,
				ped.GetHandle(),
				true, false, -1.0f);
		}

		~CommandRapidFire() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}
	};
}
