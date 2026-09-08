#include "Rendering/AutoDriveHUD.hpp"

#include "Commands/Vehicle/CommandAutoDriveHudTelemetry.hpp"
#include "Core/Pointers.hpp"
#include "Rendering/GridRenderer.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Stand
{
	using namespace Features::AutoDriveInternal;
	using namespace std::chrono_literals;

	namespace
	{
		// Named colours, and every inline colour literal below, are
		// DirectX::XMFLOAT4 now rather than ImU32/IM_COL32(...) - Col()
		// is the same 0-255-per-channel shorthand IM_COL32 was, just
		// producing this renderer's own colour type instead. This whole
		// file no longer touches ImGui for drawing at all (see
		// AutoDriveHUD.hpp's own class comment) - only ImVec2 stays, as
		// a plain math vector type for the projection/geometry work
		// below, same as every other overlay ported this session kept
		// ImVec4/ImColor/ImVec2 at their own data-model boundaries
		// rather than rewriting unrelated math.
		constexpr DirectX::XMFLOAT4 Col(int r, int g, int b, int a = 255)
		{
			return {r / 255.f, g / 255.f, b / 255.f, a / 255.f};
		}

		constexpr DirectX::XMFLOAT4 kPanel = Col(15, 22, 31, 224);
		constexpr DirectX::XMFLOAT4 kPanelBorder = Col(93, 120, 143, 140);
		constexpr DirectX::XMFLOAT4 kBlue = Col(35, 144, 255, 255);
		constexpr DirectX::XMFLOAT4 kBlueShadow = Col(3, 25, 48, 230);
		constexpr DirectX::XMFLOAT4 kLane = Col(176, 195, 210, 118);
		constexpr DirectX::XMFLOAT4 kText = Col(235, 244, 250, 255);
		constexpr DirectX::XMFLOAT4 kMuted = Col(143, 163, 178, 255);

		struct Projection
		{
			ImVec2 m_PanelMin{};
			ImVec2 m_PanelMax{};
			float m_CenterX = 0.0f;
			float m_EgoY = 0.0f;
			float m_RoadTop = 0.0f;
			float m_Width = 0.0f;

			ImVec2 Project(HudPoint point) const
			{
				if (point.m_Y >= 0.0f)
				{
					const auto normalized = std::clamp(point.m_Y / 90.0f, 0.0f, 1.0f);
					const auto depth = std::pow(normalized, 0.72f);
					const auto horizontalScale = std::lerp(m_Width / 34.0f, m_Width / 120.0f, depth);
					return {m_CenterX + point.m_X * horizontalScale, m_EgoY - depth * (m_EgoY - m_RoadTop)};
				}

				const auto behind = std::clamp(-point.m_Y / 30.0f, 0.0f, 1.0f);
				return {m_CenterX + point.m_X * (m_Width / 34.0f), m_EgoY + behind * (m_PanelMax.y - 18.0f - m_EgoY)};
			}

			float HorizontalScale(float forward) const
			{
				const auto depth = std::pow(std::clamp(std::max(0.0f, forward) / 90.0f, 0.0f, 1.0f), 0.72f);
				return std::lerp(m_Width / 34.0f, m_Width / 120.0f, depth);
			}

			float DepthFade(float forward) const
			{
				return std::clamp(1.15f - std::max(0.0f, forward) / 120.0f, 0.25f, 1.0f);
			}
		};

		struct RenderEntity
		{
			HudEntitySnapshot m_Data{};
			std::chrono::steady_clock::time_point m_LastSeen{};
		};

		// This frame's layout, computed once in Draw() (the rect pass)
		// and read back by DrawText() afterward - same "compute once,
		// two passes read/apply" contract as every other overlay ported
		// this session (see e.g. Overlay.cpp's own OverlayState). Keeps
		// the snapshot's shared_ptr alive across both passes, since only
		// Draw() calls AutoDriveHudTelemetry::GetSnapshot() itself.
		struct FrameState
		{
			bool m_Visible = false;
			std::shared_ptr<const AutoDriveHudSnapshot> m_Snapshot;
			Projection m_Projection{};
			float m_Scale = 1.0f;
		};
		FrameState g_Frame;

		DirectX::XMFLOAT4 WithAlpha(DirectX::XMFLOAT4 colour, float alpha)
		{
			colour.w = std::clamp(colour.w * alpha, 0.0f, 1.0f);
			return colour;
		}

		const char* PhaseLabel(const AutoDriveHudSnapshot& snapshot)
		{
			if (snapshot.m_RouteCalculating)
				return "CALCULATING";
			switch (snapshot.m_Phase)
			{
			case RoutePhase::Navigation: return "NAVIGATING";
			case RoutePhase::TargetLost: return "CALCULATING";
			case RoutePhase::Wander: return "ROAMING";
			case RoutePhase::Arrived: return "ARRIVING";
			default: return "STANDBY";
			}
		}

		// Same "no real curve/absolute-pixel-size rendering in this
		// pipeline" trade-off DrawCircleFilledScreen already makes -
		// converts an ImGui-style "desired pixel height" into the plain
		// scale multiplier DrawTextScreen actually takes, by measuring
		// the embedded font's own native size once.
		float PixelScale(float desiredHeightPx)
		{
			using Rendering::GridRenderer;
			static const float nativeHeight = GridRenderer::MeasureText("Ag", 1.0f).y;
			return nativeHeight > 0.0f ? desiredHeightPx / nativeHeight : 1.0f;
		}

		// Replaces the original's own borrowed ImGui::GetTime() - this
		// file no longer touches ImGui for anything but ImVec2's own
		// math operators (see this namespace's own comment above), so
		// the lane-change pulse animation can't quietly need ImGui's
		// clock to keep running either, same reasoning as Overlay.cpp's
		// own TickFps().
		float ElapsedSeconds()
		{
			static const auto start = std::chrono::steady_clock::now();
			return std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
		}

		void DrawPolylineScreen(const Projection& projection, const std::vector<HudPoint>& points, const DirectX::XMFLOAT4& colour, float thickness)
		{
			using Rendering::GridRenderer;

			std::vector<ImVec2> projected;
			projected.reserve(points.size());
			for (const auto& point : points)
			{
				if (point.m_Y >= -30.0f && point.m_Y <= 170.0f)
					projected.push_back(projection.Project(point));
			}

			for (std::size_t index = 1; index < projected.size(); ++index)
				GridRenderer::DrawLineScreen(projected[index - 1].x, projected[index - 1].y, projected[index].x, projected[index].y, colour, thickness);
		}

		void DrawRoute(const Projection& projection, const AutoDriveHudSnapshot& snapshot, float scale)
		{
			using Rendering::GridRenderer;

			if (!snapshot.m_RouteReliable || snapshot.m_RoutePoints.size() < 2)
				return;

			if (!snapshot.m_RoutePredicted)
			{
				DrawPolylineScreen(projection, snapshot.m_RoutePoints, kBlueShadow, 8.0f * scale);
				DrawPolylineScreen(projection, snapshot.m_RoutePoints, kBlue, 4.0f * scale);
				return;
			}

			for (std::size_t index = 1; index < snapshot.m_RoutePoints.size(); ++index)
			{
				if (index % 2 == 0)
					continue;
				const auto from = projection.Project(snapshot.m_RoutePoints[index - 1]);
				const auto to = projection.Project(snapshot.m_RoutePoints[index]);
				GridRenderer::DrawLineScreen(from.x, from.y, to.x, to.y, kBlueShadow, 7.0f * scale);
				GridRenderer::DrawLineScreen(from.x, from.y, to.x, to.y, WithAlpha(kBlue, 0.85f), 3.5f * scale);
			}
		}

		void DrawVehicle(const Projection& projection, const HudEntitySnapshot& entity, float alpha)
		{
			using Rendering::GridRenderer;

			const auto center = projection.Project(entity.m_Position);
			const auto pxPerMeter = projection.HorizontalScale(entity.m_Position.m_Y);
			const auto halfWidth = std::clamp(entity.m_Footprint.m_X * pxPerMeter * 0.5f, 2.5f, 15.0f);
			const auto halfLength = std::clamp(entity.m_Footprint.m_Y * pxPerMeter * 0.38f, 4.0f, 22.0f);
			const auto direction = ImVec2(std::sin(entity.m_RelativeHeadingRadians), -std::cos(entity.m_RelativeHeadingRadians));
			const auto right = ImVec2(-direction.y, direction.x);
			ImVec2 corners[4] = {
			    center + direction * halfLength + right * halfWidth,
			    center + direction * halfLength - right * halfWidth,
			    center - direction * halfLength - right * halfWidth,
			    center - direction * halfLength + right * halfWidth};

			const DirectX::XMFLOAT2 points[4] = {{corners[0].x, corners[0].y}, {corners[1].x, corners[1].y}, {corners[2].x, corners[2].y}, {corners[3].x, corners[3].y}};
			GridRenderer::DrawPolygonFilledScreen(points, 4, WithAlpha(Col(166, 181, 193, 245), alpha));

			for (int index = 0; index < 4; ++index)
				GridRenderer::DrawLineScreen(corners[index].x, corners[index].y, corners[(index + 1) % 4].x, corners[(index + 1) % 4].y, WithAlpha(Col(233, 241, 246, 235), alpha), 1.0f);

			GridRenderer::DrawLineScreen(corners[0].x, corners[0].y, corners[1].x, corners[1].y, WithAlpha(Col(235, 248, 255, 245), alpha), 2.0f);
		}

		void DrawPedestrian(const Projection& projection, const HudEntitySnapshot& entity, float alpha)
		{
			using Rendering::GridRenderer;

			const auto center = projection.Project(entity.m_Position);
			const auto size = std::clamp(projection.HorizontalScale(entity.m_Position.m_Y) * 0.42f, 2.5f, 7.0f);
			const auto colour = WithAlpha(Col(255, 184, 88, 255), alpha);
			GridRenderer::DrawCircleFilledScreen(center.x, center.y - size, size * 0.45f, colour);
			GridRenderer::DrawLineScreen(center.x, center.y - size * 0.45f, center.x, center.y + size, colour, std::max(1.0f, size * 0.35f));
		}

		void DrawTrafficLight(const Projection& projection, const HudEntitySnapshot& entity, float alpha)
		{
			using Rendering::GridRenderer;

			const auto center = projection.Project(entity.m_Position);
			const auto size = std::clamp(projection.HorizontalScale(entity.m_Position.m_Y) * 0.55f, 3.0f, 9.0f);
			GridRenderer::DrawLineScreen(center.x, center.y + size * 2.5f, center.x, center.y, WithAlpha(Col(126, 139, 148, 255), alpha), 1.5f);
			GridRenderer::DrawRectFilledScreen(center.x - size * 0.55f, center.y - size * 1.7f, center.x + size * 0.55f, center.y, WithAlpha(Col(35, 41, 46, 255), alpha));
			const auto lamp = entity.m_TrafficLightState == TrafficLightState::RedInferred ? Col(255, 65, 65, 255) : Col(132, 142, 150, 255);
			GridRenderer::DrawCircleFilledScreen(center.x, center.y - size * 1.15f, size * 0.31f, WithAlpha(lamp, alpha));
		}

		void DrawObject(const Projection& projection, const HudEntitySnapshot& entity, float alpha)
		{
			using Rendering::GridRenderer;

			const auto center = projection.Project(entity.m_Position);
			const auto pxPerMeter = projection.HorizontalScale(entity.m_Position.m_Y);
			const auto size = std::clamp(pxPerMeter * std::max(0.5f, entity.m_Footprint.m_X), 3.0f, 22.0f);
			switch (entity.m_Kind)
			{
			case HudEntityKind::Cone:
			{
				const DirectX::XMFLOAT2 points[3] = {{center.x, center.y - size}, {center.x - size * 0.65f, center.y + size * 0.55f}, {center.x + size * 0.65f, center.y + size * 0.55f}};
				GridRenderer::DrawPolygonFilledScreen(points, 3, WithAlpha(Col(255, 126, 35, 255), alpha));
				break;
			}
			case HudEntityKind::Barrier:
				GridRenderer::DrawRectFilledScreen(center.x - size, center.y - size * 0.28f, center.x + size, center.y + size * 0.28f, WithAlpha(Col(242, 153, 53, 255), alpha));
				GridRenderer::DrawLineScreen(center.x - size * 0.7f, center.y + size * 0.25f, center.x - size * 0.3f, center.y - size * 0.25f, WithAlpha(Col(244, 244, 235, 255), alpha), 2.0f);
				GridRenderer::DrawLineScreen(center.x + size * 0.1f, center.y + size * 0.25f, center.x + size * 0.5f, center.y - size * 0.25f, WithAlpha(Col(244, 244, 235, 255), alpha), 2.0f);
				break;
			case HudEntityKind::Barrel:
				GridRenderer::DrawCircleFilledScreen(center.x, center.y, size * 0.55f, WithAlpha(Col(210, 116, 46, 255), alpha));
				break;
			case HudEntityKind::Bollard:
				GridRenderer::DrawRectFilledScreen(center.x - size * 0.22f, center.y - size, center.x + size * 0.22f, center.y + size * 0.35f, WithAlpha(Col(224, 208, 93, 255), alpha));
				break;
			case HudEntityKind::Tree:
				GridRenderer::DrawLineScreen(center.x, center.y + size * 0.7f, center.x, center.y, WithAlpha(Col(113, 82, 51, 255), alpha), std::max(1.0f, size * 0.2f));
				GridRenderer::DrawCircleFilledScreen(center.x, center.y - size * 0.45f, size * 0.72f, WithAlpha(Col(67, 151, 99, 235), alpha));
				break;
			default:
				GridRenderer::DrawRectFilledScreen(center.x - size * 0.45f, center.y - size * 0.45f, center.x + size * 0.45f, center.y + size * 0.45f, WithAlpha(Col(152, 162, 169, 220), alpha));
				break;
			}
		}

		void DrawEntity(const Projection& projection, const HudEntitySnapshot& entity, float alpha)
		{
			alpha *= projection.DepthFade(entity.m_Position.m_Y);
			switch (entity.m_Kind)
			{
			case HudEntityKind::Vehicle: DrawVehicle(projection, entity, alpha); break;
			case HudEntityKind::Pedestrian: DrawPedestrian(projection, entity, alpha); break;
			case HudEntityKind::TrafficLight: DrawTrafficLight(projection, entity, alpha); break;
			default: DrawObject(projection, entity, alpha); break;
			}
		}

		void DrawEgoVehicle(const Projection& projection, float scale)
		{
			using Rendering::GridRenderer;

			const auto center = projection.Project({0.0f, 0.0f});
			const auto halfWidth = 12.0f * scale;
			const auto halfLength = 22.0f * scale;
			ImVec2 points[4] = {
			    {center.x - halfWidth * 0.7f, center.y - halfLength},
			    {center.x + halfWidth * 0.7f, center.y - halfLength},
			    {center.x + halfWidth, center.y + halfLength},
			    {center.x - halfWidth, center.y + halfLength}};

			const DirectX::XMFLOAT2 fillPoints[4] = {{points[0].x, points[0].y}, {points[1].x, points[1].y}, {points[2].x, points[2].y}, {points[3].x, points[3].y}};
			GridRenderer::DrawPolygonFilledScreen(fillPoints, 4, Col(235, 242, 247, 255));

			for (int index = 0; index < 4; ++index)
				GridRenderer::DrawLineScreen(points[index].x, points[index].y, points[(index + 1) % 4].x, points[(index + 1) % 4].y, kBlue, 2.0f * scale);

			GridRenderer::DrawRectFilledScreen(center.x - halfWidth * 0.55f, center.y - halfLength * 0.45f, center.x + halfWidth * 0.55f, center.y + halfLength * 0.1f, Col(78, 104, 125, 255));
		}

		// Divider line + pulsing lane-change arrows only - the header's
		// own text labels are DrawHeaderText() below, since this runs
		// from Draw()'s own rect pass.
		void DrawHeaderLines(const Projection& projection, const AutoDriveHudSnapshot& snapshot, float scale)
		{
			using Rendering::GridRenderer;

			GridRenderer::DrawLineScreen(projection.m_PanelMin.x + 16.0f * scale, projection.m_PanelMin.y + 70.0f * scale, projection.m_PanelMax.x - 16.0f * scale, projection.m_PanelMin.y + 70.0f * scale, Col(112, 139, 158, 80), 1.0f);

			if (snapshot.m_Maneuver == HudManeuver::None)
				return;

			const auto changingRight = snapshot.m_Maneuver == HudManeuver::LaneChangeRight;
			const auto labelY = projection.m_PanelMin.y + 78.0f * scale;
			const auto pulse = static_cast<int>(ElapsedSeconds() * 4.0f) % 3;
			for (int index = 0; index < 3; ++index)
			{
				const auto x = projection.m_CenterX + (index - 1) * 12.0f * scale;
				const auto alpha = index == pulse ? 1.0f : 0.38f;
				const auto direction = changingRight ? 1.0f : -1.0f;
				const ImVec2 tip{x + direction * 5.0f * scale, labelY + 23.0f * scale};
				GridRenderer::DrawLineScreen(x - direction * 3.0f * scale, tip.y - 5.0f * scale, tip.x, tip.y, WithAlpha(kBlue, alpha), 2.0f * scale);
				GridRenderer::DrawLineScreen(tip.x, tip.y, x - direction * 3.0f * scale, tip.y + 5.0f * scale, WithAlpha(kBlue, alpha), 2.0f * scale);
			}
		}

		void DrawHeaderText(const Projection& projection, const AutoDriveHudSnapshot& snapshot, float scale)
		{
			using Rendering::GridRenderer;

			char speed[16]{};
			std::snprintf(speed, sizeof(speed), "%d", snapshot.m_SpeedKph);
			GridRenderer::DrawTextScreen(projection.m_PanelMin.x + 20.0f * scale, projection.m_PanelMin.y + 14.0f * scale, speed, kText, PixelScale(30.0f * scale));
			GridRenderer::DrawTextScreen(projection.m_PanelMin.x + 22.0f * scale, projection.m_PanelMin.y + 47.0f * scale, "km/h", kMuted, PixelScale(12.0f * scale));

			char target[32]{};
			std::snprintf(target, sizeof(target), "TARGET %d", snapshot.m_TargetSpeedKph);
			GridRenderer::DrawTextScreen(projection.m_PanelMin.x + 88.0f * scale, projection.m_PanelMin.y + 21.0f * scale, target, kMuted, PixelScale(12.0f * scale));
			GridRenderer::DrawTextScreen(projection.m_PanelMin.x + 88.0f * scale, projection.m_PanelMin.y + 42.0f * scale, PhaseLabel(snapshot), kText, PixelScale(13.0f * scale));

			const auto owner = snapshot.m_Owner == Owner::Npc ? "NPC AUTO" : "AUTO";
			const auto ownerScale = PixelScale(13.0f * scale);
			const auto ownerSize = GridRenderer::MeasureText(owner, ownerScale);
			GridRenderer::DrawTextScreen(projection.m_PanelMax.x - ownerSize.x - 18.0f * scale, projection.m_PanelMin.y + 22.0f * scale, owner, kBlue, ownerScale);

			if (snapshot.m_Maneuver == HudManeuver::None)
				return;

			const auto changingRight = snapshot.m_Maneuver == HudManeuver::LaneChangeRight;
			const auto label = changingRight ? "CHANGING LANE RIGHT" : "CHANGING LANE LEFT";
			const auto labelScale = PixelScale(13.0f * scale);
			const auto labelSize = GridRenderer::MeasureText(label, labelScale);
			GridRenderer::DrawTextScreen(projection.m_CenterX - labelSize.x * 0.5f, projection.m_PanelMin.y + 78.0f * scale, label, kBlue, labelScale);
		}
	}

	void AutoDriveHUD::Draw()
	{
		using Rendering::GridRenderer;

		static std::unordered_map<std::uint32_t, RenderEntity> renderEntities;
		static std::chrono::steady_clock::time_point lastSnapshotTime{};

		g_Frame = {};

		const auto snapshot = AutoDriveHudTelemetry::GetSnapshot();
		const auto now = std::chrono::steady_clock::now();
		if (!snapshot || !AutoDriveHudTelemetry::IsEnabled() || !snapshot->m_Visible || snapshot->m_Suppressed
		    || snapshot->m_CapturedAt == std::chrono::steady_clock::time_point{} || now - snapshot->m_CapturedAt > 500ms)
		{
			renderEntities.clear();
			lastSnapshotTime = {};
			return;
		}

		if (snapshot->m_CapturedAt != lastSnapshotTime)
		{
			for (const auto& entity : snapshot->m_Entities)
				renderEntities[entity.m_TrackId] = {entity, now};
			lastSnapshotTime = snapshot->m_CapturedAt;
		}
		for (auto iterator = renderEntities.begin(); iterator != renderEntities.end();)
		{
			if (now - iterator->second.m_LastSeen > 150ms)
				iterator = renderEntities.erase(iterator);
			else
				++iterator;
		}

		// Real client pixels (top-left origin), same "targets the real
		// screen directly" reasoning as ESP's own world-to-screen
		// primitives - see GridRenderer.hpp's own DrawLineScreen doc
		// comment. This is a fullscreen game, so the work area is just
		// the whole client area (no OS taskbar/etc. to inset for, unlike
		// ImGui::GetMainViewport()'s own WorkPos/WorkSize this replaces).
		const auto workWidth = static_cast<float>(*Pointers.ScreenResX);
		const auto workHeight = static_cast<float>(*Pointers.ScreenResY);
		const auto scale = std::clamp(std::min(workWidth / 1920.0f, workHeight / 1080.0f), 0.70f, 1.25f);
		const auto width = std::clamp(420.0f * scale, 280.0f, 480.0f);
		const auto maximumHeight = std::max(240.0f, std::min(680.0f, workHeight - 40.0f));
		const auto height = std::clamp(600.0f * scale, std::min(360.0f, maximumHeight), maximumHeight);
		const ImVec2 panelMin{workWidth - width - 20.0f * scale, (workHeight - height) * 0.5f};
		const ImVec2 panelMax = panelMin + ImVec2{width, height};

		// Plain rect/border - no rounded corners (the original's own
		// AddRectFilled/AddRect rounding param) without a real vector-
		// graphics layer to tessellate arcs with; same "simplest thing
		// that still looks right" trade-off DrawCircleFilledScreen's own
		// doc comment already covers. No clip rect either (the
		// original's own PushClipRect/PopClipRect) - everything here is
		// already projected to stay within the panel by construction,
		// same as every other overlay ported this session not bothering
		// to clip.
		GridRenderer::DrawRectFilledScreen(panelMin.x, panelMin.y, panelMax.x, panelMax.y, kPanel);
		GridRenderer::DrawLineScreen(panelMin.x, panelMin.y, panelMax.x, panelMin.y, kPanelBorder, 1.0f);
		GridRenderer::DrawLineScreen(panelMax.x, panelMin.y, panelMax.x, panelMax.y, kPanelBorder, 1.0f);
		GridRenderer::DrawLineScreen(panelMax.x, panelMax.y, panelMin.x, panelMax.y, kPanelBorder, 1.0f);
		GridRenderer::DrawLineScreen(panelMin.x, panelMax.y, panelMin.x, panelMin.y, kPanelBorder, 1.0f);

		const Projection projection{
		    panelMin,
		    panelMax,
		    (panelMin.x + panelMax.x) * 0.5f,
		    panelMin.y + height * 0.78f,
		    panelMin.y + 108.0f * scale,
		    width};

		for (const auto& boundary : snapshot->m_LaneBoundaries)
			DrawPolylineScreen(projection, boundary, kLane, std::max(1.0f, 1.25f * scale));
		DrawRoute(projection, *snapshot, scale);

		std::vector<std::pair<float, RenderEntity*>> sorted;
		sorted.reserve(renderEntities.size());
		for (auto& [id, entity] : renderEntities)
			sorted.emplace_back(entity.m_Data.m_Position.m_Y, &entity);
		std::sort(sorted.begin(), sorted.end(), [](const auto& left, const auto& right) {
			return left.first > right.first;
		});
		for (const auto& [depth, renderEntity] : sorted)
		{
			auto entity = renderEntity->m_Data;
			const auto extrapolation = std::clamp(std::chrono::duration<float>(now - snapshot->m_CapturedAt).count(), 0.0f, 0.1f);
			entity.m_Position.m_X += entity.m_Velocity.m_X * extrapolation;
			entity.m_Position.m_Y += entity.m_Velocity.m_Y * extrapolation;
			const auto fade = 1.0f - std::clamp(std::chrono::duration<float>(now - renderEntity->m_LastSeen - 50ms).count() / 0.1f, 0.0f, 1.0f);
			DrawEntity(projection, entity, fade);
		}

		DrawEgoVehicle(projection, scale);
		DrawHeaderLines(projection, *snapshot, scale);

		g_Frame.m_Visible = true;
		g_Frame.m_Snapshot = snapshot;
		g_Frame.m_Projection = projection;
		g_Frame.m_Scale = scale;
	}

	void AutoDriveHUD::DrawText()
	{
		if (!g_Frame.m_Visible || !g_Frame.m_Snapshot)
			return;

		DrawHeaderText(g_Frame.m_Projection, *g_Frame.m_Snapshot, g_Frame.m_Scale);
	}
}
