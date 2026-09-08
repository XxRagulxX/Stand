#include "Rendering/GridItemCommandTextslider.hpp"

#include "Commands/Commands.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kButtonSize = 22.f;
		constexpr float kGap = 6.f;
		constexpr float kLabelGap = 10.f;
		constexpr float kValuePadding = 16.f;
	}

	GridItemCommandTextslider::GridItemCommandTextslider(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Command(Commands::GetCommand<CommandTextslider>(id)),
	    m_LabelOverride(std::move(labelOverride))
	{
	}

	std::string GridItemCommandTextslider::GetDescription() const
	{
		return m_Command ? m_Command->GetDescription() : std::string{};
	}

	const std::string& GridItemCommandTextslider::Label() const
	{
		static const std::string unknown = "Unknown!";
		if (!m_Command)
			return unknown;

		return m_LabelOverride.has_value() ? *m_LabelOverride : m_Command->GetLabel();
	}

	const char* GridItemCommandTextslider::CurrentOptionText() const
	{
		if (!m_Command)
			return "?";

		return m_Command->GetCurrentOption().c_str();
	}

	float GridItemCommandTextslider::MaxItemWidth() const
	{
		if (m_MaxItemWidth.has_value())
			return *m_MaxItemWidth;

		float widest = 0.f;
		if (m_Command)
		{
			for (auto& option : m_Command->GetOptions())
				widest = std::max(widest, GridRenderer::MeasureText(option.c_str()).x);
		}

		m_MaxItemWidth = widest + kValuePadding;
		return *m_MaxItemWidth;
	}

	GridItemCommandTextslider::Layout GridItemCommandTextslider::ComputeLayout() const
	{
		Layout layout;
		layout.buttonSize = kButtonSize;
		layout.valueWidth = MaxItemWidth();

		const auto labelWidth = GridRenderer::MeasureText(Label().c_str()).x;
		layout.valueX = x + 5.f + labelWidth + kLabelGap;
		layout.prevX = layout.valueX + layout.valueWidth + kGap;
		layout.nextX = layout.prevX + layout.buttonSize + kGap;
		return layout;
	}

	void GridItemCommandTextslider::draw()
	{
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemCommandTextslider::drawText()
	{
		const auto layout = ComputeLayout();

		const auto& label = Label();
		const auto labelSize = GridRenderer::MeasureText(label.c_str());
		GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), label.c_str(), Theme::kText);

		const auto* valueText = CurrentOptionText();
		const auto valueSize = GridRenderer::MeasureText(valueText);
		GridRenderer::DrawText(layout.valueX + std::max(0.f, (layout.valueWidth - valueSize.x) * 0.5f),
		    y + std::max(0.f, (height - valueSize.y) * 0.5f),
		    valueText,
		    m_Command ? Theme::kText : Theme::kError);

		const auto prevSize = GridRenderer::MeasureText("<");
		GridRenderer::DrawText(layout.prevX + std::max(0.f, (layout.buttonSize - prevSize.x) * 0.5f),
		    y + std::max(0.f, (height - prevSize.y) * 0.5f),
		    "<",
		    Theme::kText);

		const auto nextSize = GridRenderer::MeasureText(">");
		GridRenderer::DrawText(layout.nextX + std::max(0.f, (layout.buttonSize - nextSize.x) * 0.5f),
		    y + std::max(0.f, (height - nextSize.y) * 0.5f),
		    ">",
		    Theme::kText);
	}

	void GridItemCommandTextslider::onClick(int16_t cursorX, int16_t)
	{
		if (!m_Command)
			return;

		const auto layout = ComputeLayout();

		int direction = 0;
		if (cursorX >= layout.nextX && cursorX < layout.nextX + layout.buttonSize)
			direction = 1;
		else if (cursorX >= layout.prevX && cursorX < layout.prevX + layout.buttonSize)
			direction = -1;
		else
			return;

		Cycle(direction);
	}

	bool GridItemCommandTextslider::onArrow(int delta)
	{
		if (!m_Command || m_Command->GetOptions().empty())
			return false;

		Cycle(delta > 0 ? 1 : -1);
		return true;
	}

	void GridItemCommandTextslider::Cycle(int direction)
	{
		if (!m_Command)
			return;

		m_Command->Cycle(direction);
	}
}
