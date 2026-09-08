#include "Rendering/GridItemEntityPreview.hpp"

#include "Scripting/Natives.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <cmath>

namespace Stand::Rendering
{
	namespace
	{
		// Real Stand's own CommandEntityPreviews::getAdditionalOffset()
		// (a per-model configurable extra distance) has no equivalent
		// settings page here - see this class's own header comment.
		constexpr float kForwardOffset = 3.f;
		constexpr float kRotationSpeedDegreesPerTick = 1.f;
		// Real Stand's own speed_perc formula (CommandWithEntityPreview.hpp):
		// "200 - speed_perc * 150", speed_perc = min(playerSpeed/80, 1) -
		// ported verbatim, just against the player ped's own speed always
		// rather than only while driving (see this class's own header
		// comment).
		constexpr float kMaxAlpha = 200.f;
		constexpr float kAlphaFadeRange = 150.f;
		constexpr float kSpeedForFullFade = 80.f;
	}

	GridItemEntityPreview::GridItemEntityPreview(int16_t width, int16_t height) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height)
	{
	}

	GridItemEntityPreview::~GridItemEntityPreview()
	{
		DestroyPreview();
	}

	void GridItemEntityPreview::draw()
	{
		const bool focused = isKeyboardFocused();
		m_FocusTracker.Update(focused);

		if (focused)
			TickFocused();
		else if (m_Preview)
			DestroyPreview();
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

			const auto camPos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
			const auto camRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
			const float yawRad = camRot.z * (3.14159265f / 180.f);

			rage::fvector3 spawnPos{
			    camPos.x - std::sin(yawRad) * kForwardOffset,
			    camPos.y + std::cos(yawRad) * kForwardOffset,
			    camPos.z,
			};

			auto entity = CreateEntity(spawnPos);
			if (!entity.IsValid())
				return;

			m_Preview = entity;
			m_RotationDegrees = camRot.z;
			rage::fvector3 rot{};
			rot.z = m_RotationDegrees;
			m_Preview->SetRotation(rot);
		}
		else if (ShouldRotate())
		{
			m_RotationDegrees += kRotationSpeedDegreesPerTick;
			rage::fvector3 rot{};
			rot.z = m_RotationDegrees;
			m_Preview->SetRotation(rot);
		}

		const float speedPerc = std::min(Self::GetPed().GetSpeed() / kSpeedForFullFade, 1.f);
		const int alpha = static_cast<int>(kMaxAlpha - speedPerc * kAlphaFadeRange);
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
