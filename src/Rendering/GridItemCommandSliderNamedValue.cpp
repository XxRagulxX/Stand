#include "Rendering/GridItemCommandSliderNamedValue.hpp"

#include "Commands/Commands.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <stdexcept>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kButtonSize = 22.f;
		constexpr float kValueWidth = 40.f;
		constexpr float kGap = 6.f;
	}

	GridItemCommandSliderNamedValue::GridItemCommandSliderNamedValue(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride, int step) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Command(Commands::GetCommand<StandWidgets::CommandSliderNamedValue>(id)),
	    m_LabelOverride(std::move(labelOverride)),
	    m_Step(step)
	{
	}

	std::string GridItemCommandSliderNamedValue::GetDescription() const
	{
		return m_Command ? m_Command->GetDescription() : std::string{};
	}

	const std::string& GridItemCommandSliderNamedValue::Label() const
	{
		static const std::string unknown = "Unknown!";
		if (!m_Command)
			return unknown;

		return m_LabelOverride.has_value() ? *m_LabelOverride : m_Command->GetLabel();
	}

	GridItemCommandSliderNamedValue::Layout GridItemCommandSliderNamedValue::ComputeLayout() const
	{
		Layout layout;
		layout.buttonSize = kButtonSize;
		layout.valueWidth = kValueWidth;
		layout.plusX = x + width - kButtonSize;
		layout.valueX = layout.plusX - kGap - kValueWidth;
		layout.minusX = layout.valueX - kGap - kButtonSize;
		return layout;
	}

	void GridItemCommandSliderNamedValue::draw()
	{
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemCommandSliderNamedValue::drawText()
	{
		// Every centring offset below is clamped to 0 - see the identical
		// comment in GridItemToggle.cpp.
		const auto layout = ComputeLayout();

		const auto& label = Label();
		const auto labelSize = GridRenderer::MeasureText(label.c_str());
		GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), label.c_str(), Theme::kText);

		// The one difference from GridItemCommandSlider: GetDisplayText()
		// instead of a raw std::to_string() - see CommandSliderNamedValue.hpp's
		// own doc comment.
		const auto valueStr = m_Command ? m_Command->GetDisplayText() : std::string("?");
		const auto valueSize = GridRenderer::MeasureText(valueStr.c_str());
		GridRenderer::DrawText(layout.valueX + std::max(0.f, (layout.valueWidth - valueSize.x) * 0.5f),
		    y + std::max(0.f, (height - valueSize.y) * 0.5f),
		    valueStr.c_str(),
		    m_Command ? Theme::kText : Theme::kError);

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

	void GridItemCommandSliderNamedValue::onClick(int16_t cursorX, int16_t)
	{
		if (!m_Command)
			return;

		const auto layout = ComputeLayout();

		if (cursorX >= layout.plusX && cursorX < layout.plusX + layout.buttonSize)
			Step(1);
		else if (cursorX >= layout.minusX && cursorX < layout.minusX + layout.buttonSize)
			Step(-1);
		else if (cursorX >= layout.valueX && cursorX < layout.valueX + layout.valueWidth)
			OpenCommandBox();
	}

	bool GridItemCommandSliderNamedValue::onArrow(int delta)
	{
		if (!m_Command)
			return false;

		Step(delta > 0 ? 1 : -1);
		return true;
	}

	void GridItemCommandSliderNamedValue::activate()
	{
		OpenCommandBox();
	}

	void GridItemCommandSliderNamedValue::Step(int direction)
	{
		auto value = m_Command->GetState() + direction * m_Step;

		if (auto min = m_Command->GetMinimum())
			value = std::max(*min, value);
		if (auto max = m_Command->GetMaximum())
			value = std::min(*max, value);

		m_Command->SetState(value);
	}

	void GridItemCommandSliderNamedValue::OpenCommandBox()
	{
		if (!m_Command)
			return;

		auto min = m_Command->GetMinimum();
		auto max = m_Command->GetMaximum();

		std::string rangeText;
		if (min && max)
			rangeText = std::format("Range: {} to {}", *min, *max);
		else if (min)
			rangeText = std::format("Min: {}", *min);
		else if (max)
			rangeText = std::format("Max: {}", *max);

		MenuCommandBox::Open(m_Command->GetName(),
		    Label(),
		    rangeText,
		    std::to_string(m_Command->GetState()),
		    [this](const std::string& valueToken) {
			    if (!m_Command)
				    return false;

			    auto min = m_Command->GetMinimum();
			    auto max = m_Command->GetMaximum();

			    std::string lower = valueToken;
			    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
				    return std::tolower(c);
			    });

			    int value;
			    if (lower == "min" && min)
				    value = *min;
			    else if (lower == "max" && max)
				    value = *max;
			    else
			    {
				    try
				    {
					    size_t consumed = 0;
					    value = std::stoi(valueToken, &consumed);
					    if (consumed == 0)
						    return false;
				    }
				    catch (const std::exception&)
				    {
					    return false;
				    }
			    }

			    if (min)
				    value = std::max(*min, value);
			    if (max)
				    value = std::min(*max, value);

			    m_Command->SetState(value);
			    return true;
		    });
	}
}
