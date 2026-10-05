#include "Rendering/Overlay.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <chrono>

namespace Stand
{
	std::vector<OverlayLine> Overlay::s_Lines{};
	OverlayPosition          Overlay::s_Position{OverlayPosition::TopLeft};
	float                    Overlay::s_Tps{0.f};
	float                    Overlay::s_Dps{0.f};

	namespace
	{
		constexpr float kPadding     = 10.f;
		constexpr float kLineSpacing = 2.f;
		constexpr float kBaseScale   = Rendering::Theme::kTextScale;

		DirectX::XMFLOAT2 g_FreeOverlayPos{50.f, 50.f};

		struct OverlayState
		{
			std::vector<OverlayLine> lines;
			float x         = 0.f;
			float y         = 0.f;
			float width     = 0.f;
			float scale     = kBaseScale;
			bool rightAlign = false;
		};
		OverlayState g_State;
	}

	void Overlay::Draw()
	{
		using namespace Rendering;

		{
			static auto s_Last = std::chrono::steady_clock::now();
			const auto now     = std::chrono::steady_clock::now();
			const float dt     = std::chrono::duration<float>(now - s_Last).count();
			s_Last             = now;
			if (dt > 0.f)
			{
				const float instant = 1.f / dt;
				s_Dps = s_Dps <= 0.f ? instant : s_Dps * 0.9f + instant * 0.1f;
			}
		}

		g_State       = {};
		g_State.lines = s_Lines;

		if (g_State.lines.empty())
			return;

		g_State.scale = kBaseScale;

		float totalHeight = 0.f;
		for (auto& line : g_State.lines)
		{
			const auto size  = GridRenderer::MeasureText(line.text.c_str(), g_State.scale);
			g_State.width    = std::max(g_State.width, size.x);
			totalHeight     += size.y + kLineSpacing;
		}

		g_State.rightAlign = (s_Position == OverlayPosition::TopRight || s_Position == OverlayPosition::BottomRight);

		switch (s_Position)
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

		constexpr DirectX::XMFLOAT4 textColour{1.f, 1.f, 1.f, 1.f};

		float y = g_State.y;
		for (auto& line : g_State.lines)
		{
			const auto size = GridRenderer::MeasureText(line.text.c_str(), g_State.scale);
			const float x   = g_State.rightAlign ? (g_State.x + g_State.width - size.x) : g_State.x;

			GridRenderer::DrawText(
			    x, y, line.text.c_str(),
			    line.dimmed ? Theme::kPlaceholderText : textColour,
			    g_State.scale);

			y += size.y + kLineSpacing;
		}
	}
}
