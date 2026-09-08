#include "Rendering/GridItemToggle.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kIndicatorSize = 16.f;
		constexpr float kBorderWidth = 2.f;
	}

	void GridItemToggle::draw()
	{
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);

		const float indicatorX = x + width - kIndicatorSize;
		const float indicatorY = y + std::max(0.f, (height - kIndicatorSize) * 0.5f);

		GridRenderer::DrawRect(indicatorX, indicatorY, kIndicatorSize, kIndicatorSize, Theme::kText);
		GridRenderer::DrawRect(indicatorX + kBorderWidth,
		    indicatorY + kBorderWidth,
		    kIndicatorSize - kBorderWidth * 2.f,
		    kIndicatorSize - kBorderWidth * 2.f,
		    m_State ? Theme::kAccent : Theme::kPanelBackground);
	}

	void GridItemToggle::drawText()
	{
		const float textY = y + std::max(0.f, (height - GridRenderer::MeasureText(m_Label.c_str()).y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, m_Label.c_str(), Theme::kText);
	}

	void GridItemToggle::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemToggle::activate()
	{
		m_State = !m_State;
		LOGF(INFO, "[GridRenderer] Toggle '{}' toggled, now {}", m_Label, m_State ? "on" : "off");
	}
}
