#include "Rendering/GridItemButton.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/ThemeIcons.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	void GridItemButton::draw()
	{
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);

		if (m_IconSlot != IconSlot::Count)
		{
			const float iconSize = static_cast<float>(height);
			const float iconX    = static_cast<float>(x + width) - iconSize;
			const auto& tint     = isKeyboardFocused() ? Theme::kFocusTexture : Theme::kUnfocusedTexture;
			if (ThemeIcons::IsLoaded(m_IconSlot))
				ThemeIcons::QueueDraw(m_IconSlot, iconX, static_cast<float>(y), iconSize, tint);
		}
	}

	void GridItemButton::drawText()
	{
		const auto size = GridRenderer::MeasureText(m_Label.c_str());
		const float textY = y + std::max(0.f, (height - size.y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, textY, m_Label.c_str(), Theme::kText);

		if (m_ValueGetter)
		{
			const auto val = m_ValueGetter();
			if (!val.empty())
			{
				// If there's also an icon, leave room for it on the right.
				const float iconReserve = (m_IconSlot != IconSlot::Count)
				    ? static_cast<float>(height) : 0.f;
				const auto valSize = GridRenderer::MeasureText(val.c_str());
				GridRenderer::DrawText(
				    static_cast<float>(x + width) - valSize.x - iconReserve - 5.f,
				    textY,
				    val.c_str(),
				    Theme::kText);
			}
		}
	}

	void GridItemButton::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemButton::activate()
	{
		LOGF(INFO, "[GridRenderer] Button '{}' activated", m_Label);

		if (m_Action)
			m_Action();
	}
}
