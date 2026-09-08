#include "Commands/CommandToggleLegacy.hpp"
#include "Game/ControllerInputs.hpp"
#include "Commands/CommandSliderFloatLegacy.hpp"
#include "Commands/CommandSliderLegacy.hpp"
#include "Commands/LoopedCommand.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>

// Ported from stand-reference's src/Commands/Self/CommandLevitate.cpp,
// through this project's own LoopedCommand/CommandToggle/CommandSliderFloat/
// CommandSlider pattern rather than Stand's own Command-class hierarchy -
// see the earlier, simpler version of this file for why. This pass adds
// every option Stand's real Levitation exposes: pitch-aware movement,
// momentum on disable, entity pitch tilt, rotate-only-while-moving, the
// passive up/down bob, and the ground/water hover assist (using
// MISC::GET_GROUND_Z_FOR_3D_COORD/WATER::GET_WATER_HEIGHT - the same
// natives this project's own CommandTpToWaypoint.cpp already calls
// successfully, just per-tick instead of one-shot). "Show Button
// Instructions" is registered but not wired to anything yet - Stand's
// own on-screen button-prompt overlay has no equivalent here, so the
// toggle exists for parity but currently does nothing.
namespace Stand::Features
{
	constexpr float kPi = 3.14159265358979323846f;

	static CommandSliderFloatLegacy _LevitateSpeed{"levitatespeed", "Movement Speed", "How fast you move while levitating", 0.01f, 10000.0f, 1.0f};
	static CommandSliderFloatLegacy _LevitateSprintSpeed{"levitatesprintmultiplier", "Sprint Multiplier", "How fast you go while holding Sprint", 0.01f, 10000.0f, 5.0f};
	static CommandSliderFloatLegacy _LevitateAccel{"levitateaccel", "Acceleration", "Increases the speed of levitation the longer you move. Resets once you sprint or stop moving", 0.0f, 10.0f, 0.0f};
	static CommandToggleLegacy _LevitateIgnorePitch{"levitateignorepitch", "Movement Ignores Pitch", "Disables forward and backward movement affecting height depending on where you're looking", true};
	static CommandToggleLegacy _KeepMomentum{"keepmomentum", "Keep Momentum", "Carries your velocity forward for a moment after turning levitation off", false};
	static CommandToggleLegacy _LevitateApplyPitch{"levitateapplypitch", "Apply Pitch to Entity", "Tilts your character to match where the camera is looking", false};
	static CommandToggleLegacy _LevitateOnlyRotateOnMovement{"levitaterotate", "Only Rotate On Movement", "Only turns your character to face the camera while actually moving", true};
	static CommandToggleLegacy _LevitateButtonInstructions{"levitatebuttoninstructions", "Show Button Instructions", "Shows an on-screen reminder of the levitation controls (not yet implemented)", true};
	static CommandSliderFloatLegacy _LevitatePassiveMin{"levitatepassivemin", "Min Distance From Ground", "Lower bound of the passive up/down hover effect", -10000.0f, 10000.0f, 0.0f};
	static CommandSliderFloatLegacy _LevitatePassiveMax{"levitatepassivemax", "Max Distance From Ground", "Upper bound of the passive up/down hover effect", -10000.0f, 10000.0f, 0.6f};
	static CommandSliderLegacy _LevitatePassiveSpeed{"levitatepassivespeed", "Speed", "How fast the passive up/down hover effect moves", 0, 1000000, 5};
	static CommandSliderFloatLegacy _LevitateAssistUp{"levitateassistup", "Upward Force", "How fast the ground assistant pulls you up when you're below the surface", 0.0f, 10000.0f, 0.6f};
	static CommandSliderFloatLegacy _LevitateAssistDown{"levitateassistdown", "Downward Force", "How fast the ground assistant pulls you down when you're above the surface", 0.0f, 10000.0f, 0.6f};
	static CommandSliderLegacy _LevitateAssistDeadzone{"levitateassistdeadzone", "Downward Deadzone", "How far from the ground you have to be before the assistant stops trying to pull you down", 0, 100000, 13};
	static CommandSliderFloatLegacy _LevitateAssistSnap{"levitateassistsnap", "Snapping", "How close to the surface counts as \"there\" - snaps to it exactly instead of easing in", 0.0f, 10000.0f, 0.1f};

	class Levitate : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		float m_CurrentSpeed = 0.0f;
		bool m_HasLastPos = false;
		rage::fvector3 m_LastPos{};
		float m_ZExtra = 0.0f;
		bool m_ZExtraGoingDown = false;
		bool m_HasMomentum = false;
		rage::fvector3 m_MomentumPos{};
		std::chrono::steady_clock::time_point m_MomentumTime{};

		virtual void OnEnable() override
		{
			m_CurrentSpeed = _LevitateSpeed.GetState();
			m_HasLastPos = false;
			m_ZExtra = 0.0f;
			m_ZExtraGoingDown = false;
			m_HasMomentum = false;
		}

		virtual void OnDisable() override
		{
			auto ped = Self::GetPed();
			if (!ped || !ped.IsValid())
				return;

			if (_KeepMomentum.GetState() && m_HasMomentum)
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

		virtual void OnTick() override
		{
			static constexpr ControllerInputs controls[] = {ControllerInputs::INPUT_SPRINT, ControllerInputs::INPUT_JUMP, ControllerInputs::INPUT_DUCK, ControllerInputs::INPUT_MOVE_UP_ONLY, ControllerInputs::INPUT_MOVE_DOWN_ONLY, ControllerInputs::INPUT_MOVE_LEFT_ONLY, ControllerInputs::INPUT_MOVE_RIGHT_ONLY};
			for (const auto& control : controls)
				PAD::DISABLE_CONTROL_ACTION(0, static_cast<int>(control), true);

			auto ped = Self::GetPed();
			if (!ped || !ped.IsValid())
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

			if (moving && !sprinting && _LevitateAccel.GetState() != 0.0f)
				m_CurrentSpeed = std::min(m_CurrentSpeed + _LevitateAccel.GetState() / 100.0f, 12.5f);
			else
				m_CurrentSpeed = sprinting ? _LevitateSprintSpeed.GetState() : _LevitateSpeed.GetState();

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
				if (!_LevitateIgnorePitch.GetState())
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
				surfaceZ += 1.0f; // approximate ped half-height, so we hover just above the surface rather than inside it

				const float snap = _LevitateAssistSnap.GetState() / 100.0f;
				const float deadzone = static_cast<float>(_LevitateAssistDeadzone.GetState());
				if (std::abs(pos.z - surfaceZ) < snap)
				{
					pos.z = surfaceZ;
				}
				else if (pos.z - surfaceZ < deadzone)
				{
					if (pos.z < surfaceZ)
					{
						pos.z += _LevitateAssistUp.GetState() / 100.0f;
						if (pos.z > surfaceZ)
							pos.z = surfaceZ;
					}
					else if (pos.z > surfaceZ)
					{
						pos.z -= _LevitateAssistDown.GetState() / 100.0f;
						if (pos.z < surfaceZ)
							pos.z = surfaceZ;
					}
				}
			}

			const float passiveMinRaw = _LevitatePassiveMin.GetState();
			const float passiveMaxRaw = _LevitatePassiveMax.GetState();
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

				const float step = static_cast<float>(_LevitatePassiveSpeed.GetState()) / 1000.0f;
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

			if (_KeepMomentum.GetState())
			{
				m_MomentumPos = pos;
				m_MomentumTime = std::chrono::steady_clock::now();
				m_HasMomentum = true;
			}
			else
			{
				m_HasMomentum = false;
			}

			if (!_LevitateOnlyRotateOnMovement.GetState() || moved)
			{
				rage::fvector3 rot{0.0f, 0.0f, camRot.z};
				if (_LevitateApplyPitch.GetState())
					rot.x = camRot.x;
				ped.SetRotation(rot);
			}
		}
	};

	static Levitate _Levitate{"levitate", "Levitation", "Fly freely, hovering in place when still - WASD to move, Jump/Duck for up/down, hold Sprint to go faster"};
}
