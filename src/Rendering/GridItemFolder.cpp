#include "Rendering/GridItemFolder.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kArrowGap = 5.f;
	}

	void GridItemFolder::draw()
	{
		// Focused-only highlight, no fill otherwise - confirmed against
		// origin/stand-reference's own src/Menu/GridItemList.cpp: a
		// content-list row (this project's own GridItem base draws no
		// rect at all by default) only ever gets one background rect
		// drawn for it, the focused row's own focusRectColour
		// (GridItemList.cpp's own drawRectC(..., focusRectColour) call) -
		// an unfocused row gets nothing beyond the list's own single
		// shared backdrop, not a second per-row fill. The unconditional
		// "else: kPanelBackground" this used to fall back to was this
		// project's own invention, not something real Stand's row
		// rendering ever does.
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemFolder::drawText()
	{
		const auto labelSize = GridRenderer::MeasureText(m_Label.c_str());
		const float labelY = y + std::max(0.f, (height - labelSize.y) * 0.5f);
		GridRenderer::DrawText(x + 5.f, labelY, m_Label.c_str(), Theme::kText);

		const auto arrowSize = GridRenderer::MeasureText(">");
		const float arrowY = y + std::max(0.f, (height - arrowSize.y) * 0.5f);
		GridRenderer::DrawText(x + width - arrowSize.x - kArrowGap, arrowY, ">", Theme::kText);
	}

	void GridItemFolder::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemFolder::activate()
	{
		LOGF(INFO, "[GridRenderer] Folder '{}' activated", m_Label);

		MenuNavigation::Push(m_Label, m_Target);
	}
}
