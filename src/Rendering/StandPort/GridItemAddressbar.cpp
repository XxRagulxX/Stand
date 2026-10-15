#include "Rendering/StandPort/GridItemAddressbar.hpp"

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
		const float maxW = static_cast<float>(width) - Theme::kAddressbarTextXOffset * 2.f;

		std::string display = m_Title;
		const std::string& sep = Theme::kAddressSeparator;

		while (GridRenderer::MeasureText(display.c_str(), scale).x > maxW)
		{
			const auto pos = display.find(sep);
			if (pos == std::string::npos)
				break;
			display = display.substr(pos + sep.size());
		}

		const float textY = y + Theme::kAddressbarTextYOffset + std::max(0.f, (height - GridRenderer::MeasureText(display.c_str(), scale).y) * 0.5f);
		GridRenderer::DrawText(x + Theme::kAddressbarTextXOffset, textY, display.c_str(), Theme::kText, scale);
	}
}
