#include "Rendering/GridItemLiveText.hpp"

#include "Rendering/GridRenderer.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	GridItemLiveText::GridItemLiveText(int16_t width, int16_t height, std::function<std::string()> text, DirectX::XMFLOAT4 colour) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Text(std::move(text)),
	    m_Colour(colour)
	{
	}

	void GridItemLiveText::drawText()
	{
		if (!m_Text)
			return;

		const auto text = m_Text();
		const auto size = GridRenderer::MeasureText(text.c_str());
		const float textY = y + std::max(0.f, (height - size.y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, text.c_str(), m_Colour);
	}
}
