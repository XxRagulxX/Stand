#include "Rendering/StandPort/GridItemAddressbar.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/StandPort/MenuGrid.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	void GridItemAddressbar::draw()
	{
		GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemAddressbar::drawText()
	{
		const float scale = g_MenuGrid.addressbar_text.scale;
		const float xOffset = g_MenuGrid.addressbar_text.x_offset;
		const float yOffset = g_MenuGrid.addressbar_text.y_offset;

		float reservedRight = 0.f;
		if (!m_CursorPos.empty())
		{
			const auto cpSize = GridRenderer::MeasureText(m_CursorPos.c_str(), scale);
			reservedRight = cpSize.x + xOffset;
			const float textY = y + yOffset + std::max(0.f, (height - cpSize.y) * 0.5f);
			GridRenderer::DrawText(x + width - reservedRight, textY, m_CursorPos.c_str(), Theme::kText, scale);
		}

		const float maxW = static_cast<float>(width) - xOffset * 2.f - reservedRight;

		std::string display = m_Title;
		const std::string& sep = Theme::kAddressSeparator;

		while (GridRenderer::MeasureText(display.c_str(), scale).x > maxW)
		{
			const auto pos = display.find(sep);
			if (pos == std::string::npos)
				break;
			display = display.substr(pos + sep.size());
		}

		const float textY = y + yOffset + std::max(0.f, (height - GridRenderer::MeasureText(display.c_str(), scale).y) * 0.5f);
		GridRenderer::DrawText(x + xOffset, textY, display.c_str(), Theme::kText, scale);
	}
}
