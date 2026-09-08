#include "Rendering/GridItemAddressbar.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	void GridItemAddressbar::draw()
	{
		GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemAddressbar::drawText()
	{
		const float textY = y + std::max(0.f, (height - GridRenderer::MeasureText(m_Title.c_str(), Theme::kSmallTextScale).y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, m_Title.c_str(), Theme::kText, Theme::kSmallTextScale);
	}
}
