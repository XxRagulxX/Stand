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
		const float scale = Theme::kAddressbarTextScale;
		const float textY = y + Theme::kAddressbarTextYOffset + std::max(0.f, (height - GridRenderer::MeasureText(m_Title.c_str(), scale).y) * 0.5f);
		GridRenderer::DrawText(x + Theme::kAddressbarTextXOffset, textY, m_Title.c_str(), Theme::kText, scale);
	}
}
