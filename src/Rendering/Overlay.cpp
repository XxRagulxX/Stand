#include "Rendering/Overlay.hpp"

#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/CommandListSelect.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/Invoker.hpp"

#include <algorithm>
#include <chrono>
#include <format>

namespace Stand::Features
{
	void DrawBusinessOverlay(std::vector<OverlayLine>& lines);
	CommandToggleLegacy _OverlayEnabled("overlay", "Overlay Enabled", "Show an info overlay at the top left corner of the screen");
	CommandToggleLegacy _OverlayShowFPS("overlayfps", "Overlay Show FPS", "Show frame rate in the info overlay");
	CommandToggleLegacy _OverlayLock("overlaylock", "Lock Overlay Position", "Lock the overlay so it cannot be moved in free mode");

	static std::vector<std::pair<int, const char*>> g_OverlayPositionConfig = {
	    {0, "topleft"},
	    {1, "topright"},
	    {2, "bottomleft"},
	    {3, "bottomright"},
	    {4, "free"}};

	static CommandListSelect _OverlayPositionCmd("overlaypos", "Overlay Position", "Change overlay position", g_OverlayPositionConfig, 0);
}

namespace Stand
{
	namespace
	{
		constexpr float kPadding = 10.f;
		constexpr float kLineSpacing = 2.f;
		constexpr float kBaseScale = Rendering::Theme::kTextScale;

		// Original default (was ImVec2(50.f, 50.f), client pixels) - now
		// an H-space position (see GridRenderer.hpp's own class comment
		// for what that means), same reasoning as Notifications.hpp's
		// own card geometry: since Stand's virtual canvas is also
		// 1920x1080, the same numbers still mean the same thing at
		// 1080p and now actually scale on every other resolution too.
		//
		// The original could drag this around in free mode via
		// ImGui::GetWindowPos() while the window was being moved with
		// the mouse - this renderer has no mouse at all (see
		// GridRenderer::WndProcImpl's own comment on why), so Free mode
		// just draws at this fixed point instead. No keyboard
		// equivalent exists yet for repositioning it - same "no click to
		// bind this to any more" trade-off as Notifications' own
		// context-function line.
		DirectX::XMFLOAT2 g_FreeOverlayPos{50.f, 50.f};

		// Computed once per frame in Draw() (called first - see
		// Overlay.hpp's own comment on the Draw()/DrawText() split) from
		// the FPS counter and Features::DrawBusinessOverlay()'s own
		// lines, then just read back by DrawText(). Draw() itself has no
		// rect pass content to draw (this overlay has no background),
		// but still owns computing this, same "whoever the pipeline
		// happens to call first owns advancing frame state" contract
		// Notifications.cpp's own Draw() has (there rects and state-
		// advance genuinely share the pass; here it's simply nowhere
		// else more natural to put it).
		struct OverlayState
		{
			std::vector<OverlayLine> lines;
			float x = 0.f, y = 0.f; // top-left of the whole text block
			float width = 0.f;      // widest line - needed for right-aligned modes
			float scale = kBaseScale;
			bool rightAlign = false;
		};
		OverlayState g_State;

		// Replaces the original's own borrowed ImGui::GetIO().Framerate -
		// this file no longer touches ImGui for anything, so its FPS
		// counter can't lean on ImGui's frame-time tracking continuing
		// to exist either. A simple EMA
		// over real frame deltas, same smoothing feel ImGui's own
		// counter has. Ticked unconditionally at the very top of Draw()
		// every real frame (this callback already runs every frame
		// regardless of whether the overlay is even shown - see
		// GridRenderer::DrawImpl's own call site), not just while the
		// overlay/FPS line is actually enabled - otherwise re-enabling
		// either after a while would see one huge stale-delta spike.
		float TickFps()
		{
			static auto lastTime = std::chrono::steady_clock::now();
			static float fps = 0.f;

			const auto now = std::chrono::steady_clock::now();
			const float delta = std::chrono::duration<float>(now - lastTime).count();
			lastTime = now;

			if (delta > 0.f)
			{
				const float instant = 1.f / delta;
				fps = fps <= 0.f ? instant : fps * 0.9f + instant * 0.1f;
			}

			return fps;
		}
	}

	void Overlay::Draw()
	{
		using namespace Rendering;

		const float fps = TickFps();

		g_State = {};

		if (!Features::_OverlayEnabled.GetState() || !NativeInvoker::AreHandlersCached())
			return;

		if (Features::_OverlayShowFPS.GetState())
			g_State.lines.push_back({std::format("FPS: {}", static_cast<int>(fps)), false});

		Features::DrawBusinessOverlay(g_State.lines);

		if (g_State.lines.empty())
			return;

		const auto posMode = static_cast<OverlayPosition>(Features::_OverlayPositionCmd.GetState());
		g_State.scale = kBaseScale;

		float totalHeight = 0.f;
		for (auto& line : g_State.lines)
		{
			const auto size = GridRenderer::MeasureText(line.text.c_str(), g_State.scale);
			g_State.width = std::max(g_State.width, size.x);
			totalHeight += size.y + kLineSpacing;
		}

		g_State.rightAlign = (posMode == OverlayPosition::TopRight || posMode == OverlayPosition::BottomRight);

		switch (posMode)
		{
		case OverlayPosition::TopLeft:
			g_State.x = kPadding;
			g_State.y = kPadding;
			break;
		case OverlayPosition::TopRight:
			g_State.x = Theme::kHudWidth - kPadding - g_State.width;
			g_State.y = kPadding;
			break;
		case OverlayPosition::BottomLeft:
			g_State.x = kPadding;
			g_State.y = Theme::kHudHeight - kPadding - totalHeight;
			break;
		case OverlayPosition::BottomRight:
			g_State.x = Theme::kHudWidth - kPadding - g_State.width;
			g_State.y = Theme::kHudHeight - kPadding - totalHeight;
			break;
		case OverlayPosition::Free:
		default:
			g_State.x = g_FreeOverlayPos.x;
			g_State.y = g_FreeOverlayPos.y;
			break;
		}
	}

	void Overlay::DrawText()
	{
		using namespace Rendering;

		// Overlay Text Color/Scale used to be user-customizable from the
		// classic Customize page (Config/GUISettings.cpp, now removed) -
		// dropped along with it rather than ported, since Grid has no
		// equivalent settings page for them. Fixed to the original's own
		// default (red, no scaling) instead.
		constexpr DirectX::XMFLOAT4 textColour{1.f, 0.f, 0.f, 1.f};

		float y = g_State.y;
		for (auto& line : g_State.lines)
		{
			const auto size = GridRenderer::MeasureText(line.text.c_str(), g_State.scale);
			const float x = g_State.rightAlign ? (g_State.x + g_State.width - size.x) : g_State.x;

			GridRenderer::DrawText(x, y, line.text.c_str(), line.dimmed ? Theme::kPlaceholderText : textColour, g_State.scale);
			y += size.y + kLineSpacing;
		}
	}
}
