#include "Rendering/GridItemTabsHorizontal.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kTabPaddingX = 12.f;
		constexpr float kTabGap = 6.f;
	}

	void GridItemTabsHorizontal::draw()
	{
		float drawX = x;
		for (size_t i = 0; i < m_Tabs.size(); ++i)
		{
			const float tabWidth = GridRenderer::MeasureText(m_Tabs[i].c_str()).x + kTabPaddingX * 2.f;
			GridRenderer::DrawRect(
			    drawX,
			    y,
			    tabWidth,
			    height,
			    i == m_ActiveIndex ? Theme::kAccent : Theme::kPanelBackground);
			drawX += tabWidth + kTabGap;
		}
	}

	void GridItemTabsHorizontal::drawText()
	{
		float drawX = x;
		for (const auto& tab : m_Tabs)
		{
			const auto size = GridRenderer::MeasureText(tab.c_str());
			const float tabWidth = size.x + kTabPaddingX * 2.f;
			// Clamped to 0 - see the identical comment in
			// GridItemToggle.cpp: otherwise a label taller than height
			// centres upward out of this item's own row.
			const float textY = y + std::max(0.f, (height - size.y) * 0.5f);
			GridRenderer::DrawText(drawX + kTabPaddingX, textY, tab.c_str(), Theme::kText);
			drawX += tabWidth + kTabGap;
		}
	}

	float GridItemTabsHorizontal::GetTotalWidth(const std::vector<std::string>& tabs)
	{
		float totalWidth = 0.f;
		for (size_t i = 0; i < tabs.size(); ++i)
		{
			totalWidth += GridRenderer::MeasureText(tabs[i].c_str()).x + kTabPaddingX * 2.f;
			if (i + 1 < tabs.size())
				totalWidth += kTabGap;
		}
		return totalWidth;
	}

	void GridItemTabsHorizontal::onClick(int16_t cursorX, int16_t)
	{
		// Recompute the same per-tab segment boundaries draw() lays out,
		// and find which one cursorX falls into.
		float drawX = x;
		for (size_t i = 0; i < m_Tabs.size(); ++i)
		{
			const float tabWidth = GridRenderer::MeasureText(m_Tabs[i].c_str()).x + kTabPaddingX * 2.f;
			if (cursorX >= drawX && cursorX < drawX + tabWidth)
			{
				m_ActiveIndex = i;
				LOGF(INFO, "[GridRenderer] Tab '{}' clicked", m_Tabs[i]);
				return;
			}
			drawX += tabWidth + kTabGap;
		}
	}

	bool GridItemTabsHorizontal::onArrow(int delta)
	{
		if (m_Tabs.size() < 2)
			return false;

		const auto count = static_cast<int>(m_Tabs.size());
		auto index = (static_cast<int>(m_ActiveIndex) + (delta > 0 ? 1 : -1)) % count;
		if (index < 0)
			index += count;
		m_ActiveIndex = static_cast<size_t>(index);
		return true;
	}
}
