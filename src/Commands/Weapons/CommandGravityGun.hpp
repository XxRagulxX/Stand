#pragma once
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Util/Math.hpp"
#include "World/Self.hpp"

#include <cmath>

namespace Stand
{
	class CommandGravityGunToggle : public CommandToggle
	{
	public:
		CommandToggleNoCorrelation* m_contrivedPhysics = nullptr;
		CommandSliderFloat*         m_force            = nullptr;
		CommandToggleNoCorrelation* m_collision        = nullptr;

	private:
		int   m_target           = 0;
		int   m_yeetedTarget     = 0;
		float m_distanceFromSelf = 10.0f;

		void releaseTarget()
		{
			ENTITY::SET_ENTITY_COLLISION(m_target, TRUE, FALSE);
			ENTITY::SET_ENTITY_HAS_GRAVITY(m_target, TRUE);
			m_target = 0;
		}

	public:
		explicit CommandGravityGunToggle(CommandList* parent)
			: CommandToggle(parent, LIT("Gravity Gun"), CMDNAMES("gravitygun"))
		{
		}

		void onEnable(Click& click) override
		{
			CommandTickDispatch::AddCommand(this);
		}

		void onDisable(Click& click) override
		{
			CommandTickDispatch::RemoveCommand(this);
			if (m_target)
				releaseTarget();
			m_yeetedTarget = 0;
		}

		~CommandGravityGunToggle() override
		{
			CommandTickDispatch::RemoveCommand(this);
		}

		void onTick() override
		{
			if (!m_force)
				return;

			const float forceMult = m_force->getFloatValue();

			if (m_target)
			{
				if (!ENTITY::DOES_ENTITY_EXIST(m_target))
				{
					m_target = 0;
					return;
				}

				if (!m_on)
				{
					releaseTarget();
					return;
				}

				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_ATTACK))
				{
					const Vector3 camRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
					const auto    camDir = Math::RotationToDirection({ camRot.x, camRot.y, camRot.z });
					const float   scale  = 1000.0f * forceMult;
					ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(
						m_target, 1,
						camDir.x * scale, camDir.y * scale, camDir.z * scale,
						TRUE, FALSE, TRUE, TRUE);
					m_yeetedTarget = m_target;
					releaseTarget();
					return;
				}

				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_CURSOR_SCROLL_UP))
					m_distanceFromSelf += 1.0f;
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_CURSOR_SCROLL_DOWN))
					m_distanceFromSelf -= 1.0f;
				if (m_distanceFromSelf < 2.0f)
					m_distanceFromSelf = 2.0f;

				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_AIM))
				{
					const Vector3        camCoord = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
					const Vector3        camRot   = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
					const auto           camDir   = Math::RotationToDirection({ camRot.x, camRot.y, camRot.z });

					const rage::fvector3 desiredPos{
						camCoord.x + camDir.x * (m_distanceFromSelf + 2.0f),
						camCoord.y + camDir.y * (m_distanceFromSelf + 2.0f),
						camCoord.z + camDir.z * (m_distanceFromSelf + 2.0f)
					};

					const Vector3        entCoords = ENTITY::GET_ENTITY_COORDS(m_target, TRUE);
					const rage::fvector3 entPos{ entCoords.x, entCoords.y, entCoords.z };

					const float dx   = desiredPos.x - entPos.x;
					const float dy   = desiredPos.y - entPos.y;
					const float dz   = desiredPos.z - entPos.z;
					const float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

					if (m_collision && m_collision->m_on)
						ENTITY::SET_ENTITY_COLLISION(m_target, TRUE, FALSE);
					else
						ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(m_target, FALSE, FALSE);

					ENTITY::SET_ENTITY_HAS_GRAVITY(m_target, FALSE);
					NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(m_target);

					if (dist > 0.001f)
					{
						if (m_contrivedPhysics && m_contrivedPhysics->m_on)
						{
							const float step = (dist / 8.94427191f) * forceMult;
							ENTITY::SET_ENTITY_COORDS(m_target,
								entPos.x + (dx / dist) * step,
								entPos.y + (dy / dist) * step,
								entPos.z + (dz / dist) * step,
								FALSE, FALSE, FALSE, FALSE);
						}
						else
						{
							const float step = (dist / 5.47722557505f) * forceMult;
							ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(
								m_target, 1,
								(dx / dist) * step, (dy / dist) * step, (dz / dist) * step,
								TRUE, FALSE, TRUE, TRUE);
						}
					}
				}
				else
				{
					const Vector3 camRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
					const auto    camDir = Math::RotationToDirection({ camRot.x, camRot.y, camRot.z });
					const float   scale  = 10.0f * forceMult;
					ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(
						m_target, 1,
						camDir.x * scale, camDir.y * scale, camDir.z * scale,
						TRUE, FALSE, TRUE, TRUE);
					releaseTarget();
				}
			}
			else if (m_on)
			{
				if (!PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_AIM))
				{
					m_yeetedTarget = 0;
					return;
				}

				int entHandle = 0;
				if (!PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(Self::GetPlayer().GetId(), &entHandle) || !entHandle)
					return;

				if (m_yeetedTarget && entHandle == m_yeetedTarget)
					return;

				if (ENTITY::IS_ENTITY_A_PED(entHandle) && PED::IS_PED_A_PLAYER(entHandle))
					return;

				if (ENTITY::IS_ENTITY_A_PED(entHandle) && PED::IS_PED_IN_ANY_VEHICLE(entHandle, FALSE))
					entHandle = PED::GET_VEHICLE_PED_IS_IN(entHandle, FALSE);

				m_target           = entHandle;
				m_distanceFromSelf = 10.0f;
			}
		}
	};

	class CommandGravityGun : public CommandList
	{
	public:
		explicit CommandGravityGun(CommandList* parent)
			: CommandList(parent, LIT("Gravity Gun"), CMDNAMES("gravitygunlist"))
		{
			auto* toggle = createChild<CommandGravityGunToggle>();
			toggle->m_contrivedPhysics = createChild<CommandToggleNoCorrelation>(
				LIT("Contrived Physics"), CMDNAMES("gravityguncontrivedphysics"));
			toggle->m_force = createChild<CommandSliderFloat>(
				LIT("Force Multiplier"), CMDNAMES("gravitygunforce"),
				NOLABEL, 1, 1000, 100);
			toggle->m_collision = createChild<CommandToggleNoCorrelation>(
				LIT("Collision"), CMDNAMES("gravityguncollision"));
		}
	};
}
