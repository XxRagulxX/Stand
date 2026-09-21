#include "Rendering/GridItemTabsVertical.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/ThemeIcons.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	void GridItemTabsVertical::draw()
	{
		for (size_t i = 0; i < m_Entries.size(); ++i)
		{
			const float rowY = y + m_EntryHeight * static_cast<float>(i);
			GridRenderer::DrawRect(
			    x,
			    rowY,
			    width,
			    m_EntryHeight,
			    i == m_ActiveIndex ? Theme::kAccent : Theme::kPanelBackground);

			// Tab icon — mirrors Stand's TABRENDER_LICON / TABRENDER_RICON dispatch.
			// Slot index = TabSelf + entry index; slots beyond TabDebug are silently skipped.
			const auto tabSlot = static_cast<IconSlot>(static_cast<int>(IconSlot::TabSelf) + static_cast<int>(i));
			if (tabSlot < IconSlot::Count && ThemeIcons::IsLoaded(tabSlot))
			{
				const auto& tint = Theme::kTabsIconsUseTextColour
				    ? Theme::kText
				    : (i == m_ActiveIndex ? Theme::kFocusTexture : Theme::kUnfocusedTexture);
				if (Theme::kTabsShowLeftIcon)
					ThemeIcons::QueueDraw(tabSlot, static_cast<float>(x), rowY, m_EntryHeight, tint);
				if (Theme::kTabsShowRightIcon)
					ThemeIcons::QueueDraw(tabSlot, static_cast<float>(x + width) - m_EntryHeight, rowY, m_EntryHeight, tint);
			}
		}
	}

	void GridItemTabsVertical::drawText()
	{
		if (!Theme::kTabsShowName)
			return;

		const float scale = Theme::kTabsTextScale;

		for (size_t i = 0; i < m_Entries.size(); ++i)
		{
			const float rowY = y + m_EntryHeight * static_cast<float>(i);
			const auto size = GridRenderer::MeasureText(m_Entries[i].c_str(), scale);

			// Inset text when icons are shown so label doesn't overlap the sprites.
			const auto tabSlot = static_cast<IconSlot>(static_cast<int>(IconSlot::TabSelf) + static_cast<int>(i));
			const bool hasIcon = tabSlot < IconSlot::Count && ThemeIcons::IsLoaded(tabSlot);
			const float leftInset  = (Theme::kTabsShowLeftIcon  && hasIcon) ? m_EntryHeight : 0.f;
			const float rightInset = (Theme::kTabsShowRightIcon && hasIcon) ? m_EntryHeight : 0.f;
			const float availWidth = static_cast<float>(width) - leftInset - rightInset;

			float textX;
			switch (Theme::kTabsAlignment)
			{
			case Theme::TabsAlignment::Centre:
				textX = x + leftInset + (availWidth - size.x) * 0.5f;
				break;
			case Theme::TabsAlignment::Right:
				textX = x + leftInset + availWidth - size.x - Theme::kTabsTextXOffset;
				break;
			default:
				textX = x + leftInset + Theme::kTabsTextXOffset;
				break;
			}
			GridRenderer::DrawText(
			    textX,
			    rowY + Theme::kTabsTextYOffset + std::max(0.f, (m_EntryHeight - size.y) * 0.5f),
			    m_Entries[i].c_str(),
			    Theme::kText,
			    scale);
		}
	}

	void GridItemTabsVertical::onClick(int16_t, int16_t cursorY)
	{
		const auto index = static_cast<size_t>((cursorY - y) / m_EntryHeight);
		if (index < m_Entries.size())
		{
			m_ActiveIndex = index;
			LOGF(INFO, "[GridRenderer] Sidebar entry '{}' clicked", m_Entries[index]);
		}
	}

	void GridItemTabsVertical::MoveActive(int delta)
	{
		if (m_Entries.empty())
			return;

		const auto count = static_cast<int>(m_Entries.size());
		auto index = (static_cast<int>(m_ActiveIndex) + delta) % count;
		if (index < 0)
			index += count;
		m_ActiveIndex = static_cast<size_t>(index);
	}
}
