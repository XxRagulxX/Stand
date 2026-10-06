#include "Rendering/GridItemText.hpp"

#include "Rendering/GridRenderer.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	void GridItemText::drawText()
	{
		const auto size = GridRenderer::MeasureText(m_Text.c_str());
		const float textY = y + std::max(0.f, (height - size.y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, m_Text.c_str(), m_Colour);
	}
}
