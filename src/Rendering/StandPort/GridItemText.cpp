#include "Rendering/StandPort/GridItemText.hpp"

#include "Rendering/GridRenderer.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	GridItemText::GridItemText(std::string&& text, int16_t width, int16_t extra_padding, uint8_t priority, Alignment alignment_relative_to_last, GridItem* force_alignment_to)
		: GridItem(GRIDITEM_INDIFFERENT, width, 0, priority, alignment_relative_to_last, force_alignment_to),
		  m_Text(std::move(text)),
		  m_Colour({1.f, 1.f, 1.f, 1.f})
	{
	}

	void GridItemText::drawText()
	{
		const auto size = GridRenderer::MeasureText(m_Text.c_str());
		const float textY = y + std::max(0.f, (height - size.y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, m_Text.c_str(), m_Colour);
	}
}
