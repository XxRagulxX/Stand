#pragma once

#include <string>
#include <vector>

namespace Stand
{
	class Overlay
	{
	public:
		// Rect pass - this overlay has no background of its own (same
		// "just floating text" look the original ImGui window had via
		// ImGuiWindowFlags_NoBackground), so there's nothing to actually
		// draw here, but something still has to run first each frame to
		// read the FPS/business data and compute this frame's layout -
		// see Overlay.cpp's own OverlayState comment for why that's kept
		// here rather than in DrawText().
		static void Draw();

		// Text pass - draws whatever Draw() computed this frame. Must
		// run after Draw(), same contract as Notifications' own
		// Draw()/DrawText() split.
		static void DrawText();
	};

	enum class OverlayPosition
	{
		TopLeft = 0,
		TopRight,
		BottomLeft,
		BottomRight,
		Free
	};

	// One line of overlay text (the FPS line, or one of
	// Features::DrawBusinessOverlay()'s own lines) - dimmed mirrors the
	// original's ImGui::TextDisabled() vs ImGui::Text() distinction
	// ("No active Warehouse" and friends vs real data lines).
	struct OverlayLine
	{
		std::string text;
		bool dimmed = false;
	};
}
