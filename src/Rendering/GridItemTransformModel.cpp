#include "Rendering/GridItemTransformModel.hpp"

#include "Menu/Click.hpp"
#include "Ped/Ped.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "World/Self.hpp"

#include <algorithm>
#include <cmath>

namespace Stand::Rendering
{
	namespace
	{
		// Stand draws 12 edges of the entity's oriented bounding box every
		// tick using GRAPHICS::DRAW_LINE (confirmed in
		// CommandWithEntityPreview::onTickFocused() on origin/dev). Each
		// corner is transformed from model-local space to world space via
		// GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS, which accounts for the
		// entity's current position AND rotation — so the box rotates with
		// the ped as it spins.
		void DrawBoundingBox(int entHandle, const Vector3& mn, const Vector3& mx, int r, int g, int b, int a)
		{

			// 8 corners (local space → world)
			const rage::fvector3 c[8] = {
			    ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mn.x, mn.y, mn.z),
			    ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mx.x, mn.y, mn.z),
			    ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mn.x, mx.y, mn.z),
			    ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mx.x, mx.y, mn.z),
			    ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mn.x, mn.y, mx.z),
			    ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mx.x, mn.y, mx.z),
			    ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mn.x, mx.y, mx.z),
			    ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(entHandle, mx.x, mx.y, mx.z),
			};

			// 12 edges — bottom face, top face, 4 verticals
			auto line = [&](int i, int j) {
				GRAPHICS::DRAW_LINE(c[i].x, c[i].y, c[i].z, c[j].x, c[j].y, c[j].z, r, g, b, a);
			};
			line(0, 1); line(0, 2); line(1, 3); line(2, 3);
			line(4, 5); line(4, 6); line(5, 7); line(6, 7);
			line(0, 4); line(1, 5); line(2, 6); line(3, 7);
		}

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

	GridItemTransformModel::GridItemTransformModel(int16_t width, int16_t height, Stand::CommandTransformModel* cmd) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Cmd(cmd)
	{
	}

	GridItemTransformModel::~GridItemTransformModel()
	{
		StopPreview();
	}

	void GridItemTransformModel::draw()
	{
		if (Theme::kBorderWidth > 0)
		{
			const float bw = static_cast<float>(Theme::kBorderWidth);
			GridRenderer::DrawRect(x, y, width, bw, Theme::kBorderColour);
			GridRenderer::DrawRect(x, y + height - bw, width, bw, Theme::kBorderColour);
			GridRenderer::DrawRect(x, y, bw, height, Theme::kBorderColour);
			GridRenderer::DrawRect(x + width - bw, y, bw, height, Theme::kBorderColour);
		}
		if (isKeyboardFocused())
		{
			if (Theme::kCursorBorderWidth > 0)
			{
				const float bw = static_cast<float>(Theme::kCursorBorderWidth);
				GridRenderer::DrawRect(x - bw, y - bw, width + bw * 2.f, bw, Theme::kCursorBorderColour);
				GridRenderer::DrawRect(x - bw, y + height, width + bw * 2.f, bw, Theme::kCursorBorderColour);
				GridRenderer::DrawRect(x - bw, y, bw, height, Theme::kCursorBorderColour);
				GridRenderer::DrawRect(x + width, y, bw, height, Theme::kCursorBorderColour);
			}
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
		}

		if (Theme::kDisableEntityPreviews)
		{
			if (m_WasFocused)
				StopPreview();
			m_WasFocused = false;
			return;
		}

		const bool focused = isKeyboardFocused();
		if (focused && !m_WasFocused)
			StartPreview();
		else if (!focused && m_WasFocused)
			StopPreview();
		m_WasFocused = focused;
	}

	void GridItemTransformModel::StartPreview()
	{
		StopPreview();
		if (!m_Cmd)
			return;

		m_Fiber = std::make_shared<PreviewFiber>();
		auto fiber = m_Fiber;
		const Hash hash = m_Cmd->GetHash();

		FiberPool::queueJob([this, fiber, hash] {
			// Model streaming — non-blocking: request and yield each tick,
			// matching Stand's own onTickFocused() which just calls
			// REQUEST_MODEL + return (re-checked next tick) rather than
			// a blocking yield loop.
			while (!STREAMING::HAS_MODEL_LOADED(hash))
			{
				if (!fiber->active.load(std::memory_order_relaxed))
					return;
				STREAMING::REQUEST_MODEL(hash);
				Script::current()->yield();
			}
			if (!fiber->active.load(std::memory_order_relaxed))
				return;

			// Compute model dimensions for spawn offset — Stand uses
			// ent.getDimensions().max() as the base forward distance so
			// larger models appear further away and still fill the screen.
			Vector3 mn, mx;
			MISC::GET_MODEL_DIMENSIONS(hash, &mn, &mx);
			const float dimMax = std::max({ mx.x - mn.x, mx.y - mn.y, mx.z - mn.z });
			const float totalOffset = dimMax + Theme::kPreviewPedDist;

			const auto camPos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
			const auto camRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
			const auto dir    = CamRotToDir(camRot);

			const rage::fvector3 spawnPos{
			    camPos.x + dir.x * totalOffset,
			    camPos.y + dir.y * totalOffset,
			    camPos.z + dir.z * totalOffset,
			};

			// Ped::Create re-checks model loaded (it's already loaded so the
			// inner loop is skipped), then CREATE_PED and SetPosition.
			Ped ped = Ped::Create(hash, spawnPos, 0.f);
			if (!ped.IsValid())
				return;

			const int pedHandle = ped.GetHandle();

			// Face the camera — Stand adds 180 deg for peds on spawn.
			float rotDeg = camRot.z + 180.0f;
			rage::fvector3 rot{};
			rot.z = rotDeg;
			ped.SetRotation(rot);

			// Godmode so the preview ped can't die (Stand calls godmodeEnable).
			ENTITY::SET_ENTITY_INVINCIBLE(pedHandle, TRUE, FALSE);

			// Main preview loop — runs every script tick while focused.
			// Stand does the same in onTickFocused(), called from the menu
			// script's onTickInGameViewport().
			while (fiber->active.load(std::memory_order_relaxed) && isKeyboardFocused())
			{
				if (!ped.IsValid())
					break;

				// Per-tick entity maintenance matching Stand's onTickFocused():
				ENTITY::SET_ENTITY_HAS_GRAVITY(pedHandle, FALSE);
				ENTITY::SET_ENTITY_COMPLETELY_DISABLE_COLLISION(pedHandle, FALSE, FALSE);
				ENTITY::SET_CAN_CLIMB_ON_ENTITY(pedHandle, FALSE);
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(pedHandle, TRUE);
				PED::SET_PED_CONFIG_FLAG(pedHandle, 208, TRUE); // DisableExplosionReactions

				// Revive if dead (Stand calls revive() every tick for peds).
				if (PED::IS_PED_DEAD_OR_DYING(pedHandle, TRUE))
					PED::RESURRECT_PED(pedHandle);

				// Rotate every tick.
				rotDeg += Theme::kPreviewRotationSpeed;
				rot.z = rotDeg;
				ped.SetRotation(rot);

				// Reposition in front of camera every tick so the preview
				// tracks camera movement — Stand calls setOrigin(getEntityOrigin())
				// each tick which does the same.
				const auto curCamPos = CAMERA::GET_FINAL_RENDERED_CAM_COORD();
				const auto curCamRot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
				const auto curDir    = CamRotToDir(curCamRot);
				const rage::fvector3 newPos{
				    curCamPos.x + curDir.x * totalOffset,
				    curCamPos.y + curDir.y * totalOffset,
				    curCamPos.z + curDir.z * totalOffset,
				};
				ped.SetPosition(newPos);

				// Alpha fade from player speed (Stand fades only while driving;
				// we fade from any speed since we don't need vehicle ownership
				// checks for a ped-only preview list).
				int alpha = 255;
				if (!Theme::kPreviewOpaque)
				{
					const float speedPerc = std::min(Self::GetPed().GetSpeed() / 80.f, 1.f);
					alpha = static_cast<int>(200.f - speedPerc * 150.f);
				}
				ped.SetAlpha(alpha);

				// Bounding box lines in accent colour — Stand calls
				// g_preview_ent.getBox().drawLines(arColour) every tick.
				const int br = static_cast<int>(Theme::kAccent.x * 255.f);
				const int bg = static_cast<int>(Theme::kAccent.y * 255.f);
				const int bb = static_cast<int>(Theme::kAccent.z * 255.f);
				const int ba = static_cast<int>(Theme::kAccent.w * 200.f);
				DrawBoundingBox(pedHandle, mn, mx, br, bg, bb, ba);

				Script::current()->yield();
			}

			// Cleanup — Stand's getRidOfPreview() queues this via FiberPool;
			// we're already on a fiber so we can delete directly.
			if (ped.IsValid())
			{
				ENTITY::SET_ENTITY_INVINCIBLE(pedHandle, FALSE, FALSE);
				const Hash model = ped.GetModel();
				ped.Delete();
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
			}
		});
	}

	void GridItemTransformModel::StopPreview()
	{
		if (m_Fiber)
		{
			m_Fiber->active.store(false, std::memory_order_relaxed);
			m_Fiber.reset();
		}
	}

	void GridItemTransformModel::drawText()
	{
		if (!m_Cmd)
			return;

		const bool focused = isKeyboardFocused();
		const auto& textColour = focused ? Theme::kFocusText : Theme::kUnfocusedText;
		const auto label = m_Cmd->getMenuName().getLocalisedUtf8();
		const float textScale = Theme::kCommandTextScale;
		const auto labelSize = GridRenderer::MeasureText(label.c_str(), textScale);
		GridRenderer::DrawText(x + Theme::kCommandTextXOffset, y + Theme::kCommandTextYOffset + std::max(0.f, (height - labelSize.y) * 0.5f), label.c_str(), textColour, textScale);
	}

	void GridItemTransformModel::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemTransformModel::activate()
	{
		if (!m_Cmd)
			return;

		auto* cmd = m_Cmd;
		FiberPool::queueJob([cmd] {
			Stand::Click click(Stand::CLICK_MENU, Stand::TC_SCRIPT_YIELDABLE);
			cmd->onClick(click);
		});
	}
}
