#include "Rendering/GridItemLocalList.hpp"

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

	GridItemLocalList::GridItemLocalList(int16_t width, int16_t height, std::string label, std::vector<std::string> options, int initialIndex, std::function<void(int)> onChange) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Label(std::move(label)),
	    m_Options(std::move(options)),
	    m_Index(m_Options.empty() ? 0 : std::clamp(initialIndex, 0, static_cast<int>(m_Options.size()) - 1)),
	    m_OnChange(std::move(onChange))
	{
	}

	const std::string& GridItemLocalList::CurrentItemText() const
	{
		static const std::string empty;
		if (m_Options.empty() || m_Index < 0 || static_cast<size_t>(m_Index) >= m_Options.size())
			return empty;

		return m_Options[m_Index];
	}

	float GridItemLocalList::MaxItemWidth() const
	{
		if (m_MaxItemWidth.has_value())
			return *m_MaxItemWidth;

		float widest = 0.f;
		for (auto& option : m_Options)
			widest = std::max(widest, GridRenderer::MeasureText(option.c_str()).x);

		m_MaxItemWidth = widest + kValuePadding;
		return *m_MaxItemWidth;
	}

	GridItemLocalList::Layout GridItemLocalList::ComputeLayout() const
	{
		Layout layout;
		layout.buttonSize = kButtonSize;
		layout.valueWidth = MaxItemWidth();

		// Sequential from the label's own end, not anchored to the
		// item's right edge - see GridItemCommandListSelect's own class
		// comment for why.
		const auto labelWidth = GridRenderer::MeasureText(m_Label.c_str()).x;
		layout.valueX = x + 5.f + labelWidth + kLabelGap;
		layout.prevX = layout.valueX + layout.valueWidth + kGap;
		layout.nextX = layout.prevX + layout.buttonSize + kGap;
		return layout;
	}

	void GridItemLocalList::draw()
	{
		// The only rect this item draws now - see GridItemCommandListSelect.cpp's
		// identical comment for why (no per-row fill, no button fill
		// behind "<"/">" - matches real Stand's own list-select row).
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemLocalList::drawText()
	{
		const auto layout = ComputeLayout();

		const auto labelSize = GridRenderer::MeasureText(m_Label.c_str());
		GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), m_Label.c_str(), Theme::kText);

		const auto& valueText = CurrentItemText();
		const auto valueSize = GridRenderer::MeasureText(valueText.c_str());
		GridRenderer::DrawText(layout.valueX + std::max(0.f, (layout.valueWidth - valueSize.x) * 0.5f),
		    y + std::max(0.f, (height - valueSize.y) * 0.5f),
		    valueText.c_str(),
		    m_Options.empty() ? Theme::kError : Theme::kText);

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

	void GridItemLocalList::onClick(int16_t cursorX, int16_t)
	{
		if (m_Options.empty())
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

	bool GridItemLocalList::onArrow(int delta)
	{
		if (m_Options.empty())
			return false;

		Cycle(delta > 0 ? 1 : -1);
		return true;
	}

	void GridItemLocalList::Cycle(int direction)
	{
		const auto count = static_cast<int>(m_Options.size());
		if (count == 0)
			return;

		m_Index = ((m_Index + direction) % count + count) % count;

		if (m_OnChange)
			m_OnChange(m_Index);
	}
}
