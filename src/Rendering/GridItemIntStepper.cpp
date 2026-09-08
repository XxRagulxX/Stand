#include "Rendering/GridItemIntStepper.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kButtonSize = 22.f;
		constexpr float kValueWidth = 40.f;
		constexpr float kGap = 6.f;
	}

	GridItemIntStepper::Layout GridItemIntStepper::ComputeLayout() const
	{
		Layout layout;
		layout.buttonSize = kButtonSize;
		layout.valueWidth = kValueWidth;
		layout.plusX = x + width - kButtonSize;
		layout.valueX = layout.plusX - kGap - kValueWidth;
		layout.minusX = layout.valueX - kGap - kButtonSize;
		return layout;
	}

	void GridItemIntStepper::draw()
	{
		// Full-row keyboard-focus highlight - the only rect this item
		// itself draws now (the value box and "<"/">" arrows are plain
		// text, no background of their own - matches real Stand's own
		// look, which draws no button fill behind its step arrows
		// either, only ever a per-row highlight).
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemIntStepper::drawText()
	{
		// Every centring offset below is clamped to 0 - see the identical
		// comment in GridItemToggle.cpp: otherwise text taller/wider than
		// its own box centres outside that box (upward, or into a
		// neighbouring button/box for the horizontal ones).
		const auto layout = ComputeLayout();

		const auto labelSize = GridRenderer::MeasureText(m_Label.c_str());
		GridRenderer::DrawText(
		    x,
		    y + std::max(0.f, (height - labelSize.y) * 0.5f),
		    m_Label.c_str(),
		    Theme::kText);

		const auto valueStr = std::to_string(m_Value);
		const auto valueSize = GridRenderer::MeasureText(valueStr.c_str());
		GridRenderer::DrawText(layout.valueX + std::max(0.f, (layout.valueWidth - valueSize.x) * 0.5f),
		    y + std::max(0.f, (height - valueSize.y) * 0.5f),
		    valueStr.c_str(),
		    Theme::kText);

		const auto minusSize = GridRenderer::MeasureText("<");
		GridRenderer::DrawText(layout.minusX + std::max(0.f, (layout.buttonSize - minusSize.x) * 0.5f),
		    y + std::max(0.f, (height - minusSize.y) * 0.5f),
		    "<",
		    Theme::kText);

		const auto plusSize = GridRenderer::MeasureText(">");
		GridRenderer::DrawText(layout.plusX + std::max(0.f, (layout.buttonSize - plusSize.x) * 0.5f),
		    y + std::max(0.f, (height - plusSize.y) * 0.5f),
		    ">",
		    Theme::kText);
	}

	void GridItemIntStepper::onClick(int16_t cursorX, int16_t)
	{
		const auto layout = ComputeLayout();
		const auto previous = m_Value;

		if (cursorX >= layout.plusX && cursorX < layout.plusX + layout.buttonSize)
			m_Value = std::min(m_Max, m_Value + 1);
		else if (cursorX >= layout.minusX && cursorX < layout.minusX + layout.buttonSize)
			m_Value = std::max(m_Min, m_Value - 1);

		if (m_OnChange && m_Value != previous)
			m_OnChange(m_Value);
	}

	bool GridItemIntStepper::onArrow(int delta)
	{
		const auto previous = m_Value;

		if (delta > 0)
			m_Value = std::min(m_Max, m_Value + 1);
		else if (delta < 0)
			m_Value = std::max(m_Min, m_Value - 1);

		if (m_OnChange && m_Value != previous)
			m_OnChange(m_Value);

		return true;
	}
}
