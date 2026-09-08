#include "Rendering/GridItemScrollbar.hpp"

#include "Rendering/Grid.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	void GridItemScrollbar::draw()
	{
		if (!m_View)
			return;

		GridRenderer::DrawRect(x, y, width, height, Theme::kPanelBackground);

		int16_t x1, y1, x2, y2;
		m_View->getBounds(x1, y1, x2, y2);
		const auto contentHeight = static_cast<float>(y2 - y1);
		if (contentHeight <= static_cast<float>(height))
			return;

		const auto thumbHeight = std::max(1.f, static_cast<float>(height) * static_cast<float>(height) / contentHeight);
		const auto maxOffset = contentHeight - static_cast<float>(height);
		const auto thumbOffset = (static_cast<float>(m_View->GetScrollOffset()) / maxOffset) * (static_cast<float>(height) - thumbHeight);

		GridRenderer::DrawRect(x, y + thumbOffset, width, thumbHeight, Theme::kAccent);
	}
}
