#include "Commands/Self/CommandLevitate.hpp"

#include "Game/ControllerInputs.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>

namespace Stand
{
	constexpr float kPi = 3.14159265358979323846f;

	CommandLevitate::CommandLevitate(
		CommandList* parent,
		CommandSlider* speed,
		CommandSlider* sprint_speed,
		CommandSlider* acceleration,
		CommandToggle* ignore_pitch,
		CommandToggle* keep_momentum,
		CommandToggle* apply_pitch_to_entity,
		CommandToggle* only_rotate_on_movement,
		CommandToggle* show_button_instructions,
		CommandSlider* passive_min,
		CommandSlider* passive_max,
		CommandSlider* passive_speed,
		CommandSlider* assist_up,
		CommandSlider* assist_down,
		CommandSlider* assist_deadzone,
		CommandSlider* assist_snap
	) :
		CommandToggle(parent, LIT("Levitation"), CMDNAMES("levitate", "levitation"), LIT("Fly freely, hovering in place when still. WASD to move, Jump/Duck for up/down, hold Sprint to go faster.")),
		base_speed(speed),
		sprint_speed(sprint_speed),
		acceleration(acceleration),
		ignore_pitch(ignore_pitch),
		keep_momentum(keep_momentum),
		apply_pitch_to_entity(apply_pitch_to_entity),
		only_rotate_on_movement(only_rotate_on_movement),
		show_button_instructions(show_button_instructions),
		passive_min(passive_min),
		passive_max(passive_max),
		passive_speed(passive_speed),
		assist_up(assist_up),
		assist_down(assist_down),
		assist_deadzone(assist_deadzone),
		assist_snap(assist_snap)
	{
	}

	void CommandLevitate::onEnable(Click& click)
	{
		m_HasLastPos = false;
		m_ZExtra = 0.0f;
		m_ZExtraGoingDown = false;
		m_HasMomentum = false;
		m_CurrentSpeed = base_speed->getFloatValue();
		CommandTickDispatch::AddCommand(this);
	}

	void CommandLevitate::onDisable(Click& click)
	{
		CommandTickDispatch::RemoveCommand(this);
		auto ped = Stand::Self::GetPed();
		if (!ped)
			return;

		if (keep_momentum->m_on && m_HasMomentum)
		{
			const auto elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - m_MomentumTime).count();
			if (elapsed > 0.01f)
			{
				const auto delta = (ped.GetPosition() - m_MomentumPos) * (1.0f / elapsed);
				ped.SetVelocity(delta);
			}
		}

		ped.SetFrozen(false);
	}

	void CommandLevitate::onTick()
	{
		static constexpr ControllerInputs controls[] = {
			ControllerInputs::INPUT_SPRINT,
			ControllerInputs::INPUT_JUMP,
			ControllerInputs::INPUT_DUCK,
			ControllerInputs::INPUT_MOVE_UP_ONLY,
			ControllerInputs::INPUT_MOVE_DOWN_ONLY,
			ControllerInputs::INPUT_MOVE_LEFT_ONLY,
			ControllerInputs::INPUT_MOVE_RIGHT_ONLY,
		};
		for (const auto& control : controls)
			PAD::DISABLE_CONTROL_ACTION(0, static_cast<int>(control), true);

		auto ped = Stand::Self::GetPed();
		if (!ped)
			return;

		rage::fvector3 dir{};
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_UP_ONLY))
			dir.y += 1.0f;
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_DOWN_ONLY))
			dir.y -= 1.0f;
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_LEFT_ONLY))
			dir.x -= 1.0f;
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_MOVE_RIGHT_ONLY))
			dir.x += 1.0f;
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_JUMP))
			dir.z += 1.0f;
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_DUCK))
			dir.z -= 1.0f;

		const bool sprinting = PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)ControllerInputs::INPUT_SPRINT);
		const bool moving = (dir.x != 0.0f || dir.y != 0.0f || dir.z != 0.0f);

		if (moving && !sprinting && acceleration->getFloatValue() != 0.0f)
			m_CurrentSpeed = std::min(m_CurrentSpeed + acceleration->getFloatValue() / 100.0f, 12.5f);
		else
			m_CurrentSpeed = sprinting ? sprint_speed->getFloatValue() : base_speed->getFloatValue();

		ped.SetFrozen(true);

		auto pos = m_HasLastPos ? m_LastPos : ped.GetPosition();
		pos.z -= m_ZExtra;

		const auto camRot = CAMERA::GET_GAMEPLAY_CAM_ROT(2);
		const float yawRad = camRot.z * (kPi / 180.0f);
		const float pitchRad = camRot.x * (kPi / 180.0f);

		bool moved = false;
		if (moving)
		{
			const rage::fvector3 right{std::cos(yawRad), std::sin(yawRad), 0.0f};
			rage::fvector3 forward{-std::sin(yawRad), std::cos(yawRad), 0.0f};
			if (!ignore_pitch->m_on)
				forward = rage::fvector3{-std::sin(yawRad) * std::cos(pitchRad), std::cos(yawRad) * std::cos(pitchRad), std::sin(pitchRad)};

			rage::fvector3 move = right * (dir.x * m_CurrentSpeed) + forward * (dir.y * m_CurrentSpeed);
			move.z += dir.z * m_CurrentSpeed;
			pos = pos + move;
			moved = true;
		}

		float groundZ;
		const bool hasGround = MISC::GET_GROUND_Z_FOR_3D_COORD(pos.x, pos.y, pos.z, &groundZ, false, false);
		float waterZ;
		const bool hasWater = WATER::GET_WATER_HEIGHT(pos.x, pos.y, pos.z, &waterZ);

		if (hasGround || hasWater)
		{
			float surfaceZ = (hasWater && (!hasGround || waterZ > groundZ)) ? waterZ : groundZ;
			surfaceZ += 1.0f;

			const float snap = assist_snap->getFloatValue() / 100.0f;
			const float deadzone = static_cast<float>(assist_deadzone->value);
			if (std::abs(pos.z - surfaceZ) < snap)
			{
				pos.z = surfaceZ;
			}
			else if (pos.z - surfaceZ < deadzone)
			{
				if (pos.z < surfaceZ)
				{
					pos.z += assist_up->getFloatValue() / 100.0f;
					if (pos.z > surfaceZ)
						pos.z = surfaceZ;
				}
				else if (pos.z > surfaceZ)
				{
					pos.z -= assist_down->getFloatValue() / 100.0f;
					if (pos.z < surfaceZ)
						pos.z = surfaceZ;
				}
			}
		}

		const float passiveMinRaw = passive_min->getFloatValue();
		const float passiveMaxRaw = passive_max->getFloatValue();
		if (passiveMinRaw == passiveMaxRaw)
		{
			m_ZExtra = passiveMinRaw;
		}
		else
		{
			float passiveMin = passiveMinRaw;
			float passiveMax = passiveMaxRaw;
			if (passiveMax < passiveMin)
				std::swap(passiveMin, passiveMax);

			const float step = static_cast<float>(passive_speed->value) / 1000.0f;
			if (m_ZExtraGoingDown)
			{
				m_ZExtra -= step;
				if (m_ZExtra <= passiveMin)
					m_ZExtraGoingDown = false;
			}
			else
			{
				m_ZExtra += step;
				if (m_ZExtra >= passiveMax)
					m_ZExtraGoingDown = true;
			}
		}
		pos.z += m_ZExtra;

		ped.SetPosition(pos);
		m_LastPos = pos;
		m_HasLastPos = true;

		if (keep_momentum->m_on)
		{
			m_MomentumPos = pos;
			m_MomentumTime = std::chrono::steady_clock::now();
			m_HasMomentum = true;
		}
		else
		{
			m_HasMomentum = false;
		}

		if (!only_rotate_on_movement->m_on || moved)
		{
			rage::fvector3 rot{0.0f, 0.0f, camRot.z};
			if (apply_pitch_to_entity->m_on)
				rot.x = camRot.x;
			ped.SetRotation(rot);
		}
	}

	CommandLevitate::~CommandLevitate()
	{
		CommandTickDispatch::RemoveCommand(this);
	}
}
