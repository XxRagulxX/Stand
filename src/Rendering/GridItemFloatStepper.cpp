#include "Rendering/GridItemFloatStepper.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kButtonSize = 22.f;
		constexpr float kValueWidth = 56.f; // wider than GridItemIntStepper's - "-12.34" needs more room than an int
		constexpr float kGap = 6.f;
	}

	GridItemFloatStepper::Layout GridItemFloatStepper::ComputeLayout() const
	{
		Layout layout;
		layout.buttonSize = kButtonSize;
		layout.valueWidth = kValueWidth;
		layout.plusX = x + width - kButtonSize;
		layout.valueX = layout.plusX - kGap - kValueWidth;
		layout.minusX = layout.valueX - kGap - kButtonSize;
		return layout;
	}

	void GridItemFloatStepper::draw()
	{
		// Full-row keyboard-focus highlight - see the identical comment
		// in GridItemIntStepper.cpp for why this is the only rect drawn
		// here now.
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemFloatStepper::drawText()
	{
		const auto layout = ComputeLayout();

		const auto labelSize = GridRenderer::MeasureText(m_Label.c_str());
		GridRenderer::DrawText(
		    x,
		    y + std::max(0.f, (height - labelSize.y) * 0.5f),
		    m_Label.c_str(),
		    Theme::kText);

		const auto valueStr = std::format("{:.2f}", m_Value);
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

	void GridItemFloatStepper::onClick(int16_t cursorX, int16_t)
	{
		const auto layout = ComputeLayout();

		if (cursorX >= layout.plusX && cursorX < layout.plusX + layout.buttonSize)
			Step(1);
		else if (cursorX >= layout.minusX && cursorX < layout.minusX + layout.buttonSize)
			Step(-1);
	}

	bool GridItemFloatStepper::onArrow(int delta)
	{
		Step(delta > 0 ? 1 : -1);
		return true;
	}

	void GridItemFloatStepper::Step(int direction)
	{
		const auto previous = m_Value;

		m_Value = std::clamp(m_Value + static_cast<float>(direction) * m_Step, m_Min, m_Max);

		if (m_OnChange && m_Value != previous)
			m_OnChange(m_Value);
	}
}
