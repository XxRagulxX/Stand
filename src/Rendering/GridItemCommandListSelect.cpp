#include "Rendering/GridItemCommandListSelect.hpp"

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

	GridItemCommandListSelect::GridItemCommandListSelect(int16_t width, int16_t height, joaat_t id, std::optional<std::string> labelOverride) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Command(Commands::GetCommand<CommandListSelect>(id)),
	    m_LabelOverride(std::move(labelOverride))
	{
	}

	std::string GridItemCommandListSelect::GetDescription() const
	{
		return m_Command ? m_Command->GetDescription() : std::string{};
	}

	const std::string& GridItemCommandListSelect::Label() const
	{
		static const std::string unknown = "Unknown!";
		if (!m_Command)
			return unknown;

		return m_LabelOverride.has_value() ? *m_LabelOverride : m_Command->GetLabel();
	}

	const char* GridItemCommandListSelect::CurrentItemText() const
	{
		if (!m_Command)
			return "?";

		const auto state = m_Command->GetState();
		for (auto& entry : m_Command->GetList())
		{
			if (entry.first == state)
				return entry.second;
		}

		// Matches ListCommandItem::Draw()'s own fallback (m_SelectedItem
		// defaults to "") for a state with no matching list entry.
		return "";
	}

	float GridItemCommandListSelect::MaxItemWidth() const
	{
		if (m_MaxItemWidth.has_value())
			return *m_MaxItemWidth;

		float widest = 0.f;
		if (m_Command)
		{
			for (auto& entry : m_Command->GetList())
				widest = std::max(widest, GridRenderer::MeasureText(entry.second).x);
		}

		m_MaxItemWidth = widest + kValuePadding;
		return *m_MaxItemWidth;
	}

	GridItemCommandListSelect::Layout GridItemCommandListSelect::ComputeLayout() const
	{
		Layout layout;
		layout.buttonSize = kButtonSize;
		layout.valueWidth = MaxItemWidth();

		// Sequential from the label's own end, not anchored to the
		// item's right edge - see the class comment in the header for
		// why (a wide option label would otherwise run back underneath
		// the label text on the left).
		const auto labelWidth = GridRenderer::MeasureText(Label().c_str()).x;
		layout.valueX = x + 5.f + labelWidth + kLabelGap;
		layout.prevX = layout.valueX + layout.valueWidth + kGap;
		layout.nextX = layout.prevX + layout.buttonSize + kGap;
		return layout;
	}

	void GridItemCommandListSelect::draw()
	{
		// The only rect this item draws now - real Stand's own list-select
		// row has no background of its own either (no per-row fill, no
		// button fill behind its "<"/">" - see GridItemCommandSlider.cpp's
		// own comment for the confirmed-against-source reasoning, which
		// applies identically here).
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);
	}

	void GridItemCommandListSelect::drawText()
	{
		// Every centring offset below is clamped to 0 - see the identical
		// comment in GridItemToggle.cpp: otherwise text taller/wider than
		// its own box centres outside that box (upward, or into a
		// neighbouring button/box for the horizontal ones).
		const auto layout = ComputeLayout();

		const auto& label = Label();
		const auto labelSize = GridRenderer::MeasureText(label.c_str());
		GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), label.c_str(), Theme::kText);

		const auto* valueText = CurrentItemText();
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

	void GridItemCommandListSelect::onClick(int16_t cursorX, int16_t)
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

	bool GridItemCommandListSelect::onArrow(int delta)
	{
		if (!m_Command || m_Command->GetList().empty())
			return false;

		Cycle(delta > 0 ? 1 : -1);
		return true;
	}

	void GridItemCommandListSelect::Cycle(int direction)
	{
		auto& list = m_Command->GetList();
		if (list.empty())
			return;

		const auto state = m_Command->GetState();
		auto it = std::find_if(list.begin(), list.end(), [state](auto& entry) {
			return entry.first == state;
		});

		const size_t count = list.size();
		size_t index = (it != list.end()) ? static_cast<size_t>(it - list.begin()) : 0;
		// index/count are unsigned, so "index + direction" alone would
		// underflow going backwards from 0. Casting direction (+1/-1) to
		// size_t wraps -1 to SIZE_MAX, which is -1 mod 2^64 - adding that
		// to (index + count) and reducing mod count gives exactly
		// (index - 1) mod count, the wraparound-safe decrement.
		index = (index + count + static_cast<size_t>(direction)) % count;

		m_Command->SetState(list[index].first);
	}
}
