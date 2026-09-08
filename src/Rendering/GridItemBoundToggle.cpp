#include "Rendering/GridItemBoundToggle.hpp"

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

	GridItemBoundToggle::GridItemBoundToggle(int16_t width, int16_t height, std::string label, std::function<bool()> getter, std::function<void(bool)> setter) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Label(std::move(label)),
	    m_Getter(std::move(getter)),
	    m_Setter(std::move(setter))
	{
	}

	void GridItemBoundToggle::draw()
	{
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);

		const float indicatorX = x + width - kIndicatorSize;
		const float indicatorY = y + std::max(0.f, (height - kIndicatorSize) * 0.5f);

		const auto borderColour = m_Getter ? Theme::kText : Theme::kError;
		const auto fillColour = !m_Getter ? Theme::kError : (m_Getter() ? Theme::kAccent : Theme::kPanelBackground);
		GridRenderer::DrawRect(indicatorX, indicatorY, kIndicatorSize, kIndicatorSize, borderColour);
		GridRenderer::DrawRect(indicatorX + kBorderWidth,
		    indicatorY + kBorderWidth,
		    kIndicatorSize - kBorderWidth * 2.f,
		    kIndicatorSize - kBorderWidth * 2.f,
		    fillColour);
	}

	void GridItemBoundToggle::drawText()
	{
		const float textY = y + std::max(0.f, (height - GridRenderer::MeasureText(m_Label.c_str()).y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, m_Label.c_str(), Theme::kText);
	}

	void GridItemBoundToggle::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemBoundToggle::activate()
	{
		if (!m_Getter || !m_Setter)
			return;

		m_Setter(!m_Getter());
	}
}
