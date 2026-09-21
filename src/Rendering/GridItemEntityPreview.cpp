#include "Rendering/GridItemEntityPreview.hpp"

#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <cmath>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kMaxAlpha = 200.f;
		constexpr float kAlphaFadeRange = 150.f;

		// Camera rotation (x=pitch, z=yaw, degrees) → world-space forward
		// direction vector, matching Stand's own v3::toDir().
		rage::fvector3 CamRotToDir(const Vector3& rot)
		{
			const float pitchRad = rot.x * (3.14159265f / 180.f);
			const float yawRad   = rot.z * (3.14159265f / 180.f);
			return {
			    -std::sin(yawRad) * std::cos(pitchRad),
			     std::cos(yawRad) * std::cos(pitchRad),
			     std::sin(pitchRad),
			};
		}
	}

	GridItemEntityPreview::GridItemEntityPreview(int16_t width, int16_t height) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Control(std::make_shared<PreviewControl>())
	{
	}

	GridItemEntityPreview::~GridItemEntityPreview()
	{
		m_Control->alive.store(false, std::memory_order_seq_cst);

		if (m_Preview)
		{
			const int h = m_Preview->GetHandle();
			m_Preview.reset();
			FiberPool::queueJob([h] {
				int hLocal = h;
				if (!ENTITY::DOES_ENTITY_EXIST(hLocal))
					return;
				const auto model = ENTITY::GET_ENTITY_MODEL(hLocal);
				if (!ENTITY::IS_ENTITY_A_MISSION_ENTITY(hLocal))
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(hLocal, true, true);
				ENTITY::DELETE_ENTITY(&hLocal);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
			});
		}
	}

	void GridItemEntityPreview::draw()
	{
		const bool kbFocused = isKeyboardFocused();
		if (!kbFocused)
			m_Control->suppressed.store(false, std::memory_order_relaxed);

		const bool effectiveFocused = !Theme::kDisableEntityPreviews && kbFocused
		                              && !m_Control->suppressed.load(std::memory_order_relaxed);
		m_FocusTracker.Update(effectiveFocused);
		m_Control->focused.store(effectiveFocused, std::memory_order_relaxed);

		if (effectiveFocused && !m_Control->jobPending.exchange(true))
			FiberPool::queueJob([this, ctrl = m_Control] { runWatchdog(ctrl); });
	}

	void GridItemEntityPreview::ClearPreview()
	{
		m_Control->suppressed.store(true, std::memory_order_relaxed);
		m_Control->focused.store(false, std::memory_order_relaxed);
		if (!m_Control->jobPending.exchange(true))
			FiberPool::queueJob([this, ctrl = m_Control] { runWatchdog(ctrl); });
	}

	void GridItemEntityPreview::runWatchdog(std::shared_ptr<PreviewControl> ctrl)
	{
		const bool wasFocused = ctrl->focused.exchange(false, std::memory_order_relaxed);
		ctrl->jobPending.store(false, std::memory_order_relaxed);

		if (!ctrl->alive.load(std::memory_order_acquire))
			return;

		if (wasFocused)
			TickFocused();
		else if (m_Preview)
			DestroyPreview();

		if (!ctrl->alive.load(std::memory_order_acquire))
			return;

		if (m_Preview && !ctrl->jobPending.exchange(true))
			FiberPool::queueJob([this, ctrl] { runWatchdog(ctrl); });
	}

	void GridItemEntityPreview::TickFocused()
	{
		if (m_Preview && !m_Preview->IsValid())
			m_Preview.reset();

		if (!m_Preview)
		{
			const auto modelHash = GetModelHash();
			if (modelHash == 0)
				return;

			if (!STREAMING::HAS_MODEL_LOADED(modelHash))
			{
				STREAMING::REQUEST_MODEL(modelHash);
				return;
			}

			m_TotalOffset = GetAdditionalOffset();

			const auto camPos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
			const auto camRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
			const auto fwd = CamRotToDir(camRot);
			const rage::fvector3 spawnPos{
			    camPos.x + fwd.x * m_TotalOffset,
			    camPos.y + fwd.y * m_TotalOffset,
			    camPos.z + fwd.z * m_TotalOffset,
			};

			auto entity = CreateEntity(spawnPos);
			if (!entity.IsValid())
				return;

			m_Preview = entity;
			m_RotationDegrees = camRot.z + GetInitialRotationOffset();
			rage::fvector3 rot{};
			rot.z = m_RotationDegrees;
			m_Preview->SetRotation(rot);
		}
		else
		{
			if (ShouldRotate())
			{
				m_RotationDegrees += Theme::kPreviewRotationSpeed;
				rage::fvector3 rot{};
				rot.z = m_RotationDegrees;
				m_Preview->SetRotation(rot);
			}

			const auto curCamPos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
			const auto curCamRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
			const auto fwd = CamRotToDir(curCamRot);
			m_Preview->SetPosition({
			    curCamPos.x + fwd.x * m_TotalOffset,
			    curCamPos.y + fwd.y * m_TotalOffset,
			    curCamPos.z + fwd.z * m_TotalOffset,
			});
		}

		const int h = m_Preview->GetHandle();
		ENTITY::SET_ENTITY_HAS_GRAVITY(h, false);
		ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(h, false, false);
		ENTITY::SET_CAN_CLIMB_ON_ENTITY(h, false);
		OnPreviewTick(*m_Preview);

		int alpha;
		if (Theme::kPreviewOpaque)
		{
			alpha = 255;
		}
		else
		{
			float speed_perc = 0.0f;
			auto playerVeh = Self::GetVehicle();
			if (playerVeh.IsValid())
			{
				const int driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(playerVeh.GetHandle(), -1, false);
				if (driver == Self::GetPed().GetHandle())
					speed_perc = std::min(ENTITY::GET_ENTITY_SPEED(Self::GetPed().GetHandle()) / 80.0f, 1.0f);
			}
			alpha = static_cast<int>(kMaxAlpha - speed_perc * kAlphaFadeRange);
		}
		m_Preview->SetAlpha(alpha);
	}

	void GridItemEntityPreview::DestroyPreview()
	{
		if (!m_Preview)
			return;

		const auto model = m_Preview->GetModel();
		m_Preview->Delete();
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		m_Preview.reset();
	}
}
