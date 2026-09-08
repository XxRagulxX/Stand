#include "Rendering/GridItemSelectList.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <cctype>
#include <windows.h>

namespace Stand::Rendering
{
	GridItemSelectList::GridItemSelectList(int16_t width,
	    std::string placeholder,
	    std::function<std::vector<std::string>()> rows,
	    std::function<void(size_t, const std::string&, bool, bool, bool)> onSelect,
	    int16_t maxRows) :
	    GridItem(GRIDITEM_INDIFFERENT, width, static_cast<int16_t>(Theme::kContentItemHeight * (maxRows + 1))),
	    m_MaxRows(maxRows),
	    m_Placeholder(std::move(placeholder)),
	    m_Rows(std::move(rows)),
	    m_OnSelect(std::move(onSelect))
	{
	}

	std::vector<GridItemSelectList::FilteredRow> GridItemSelectList::FilteredRows() const
	{
		std::vector<FilteredRow> result;
		if (!m_Rows)
			return result;

		auto rows = m_Rows();

		// Case-insensitive substring match - same "simplest interaction
		// that still works" trade-off as everywhere else in this system
		// without a real dropdown/fuzzy-match (see e.g.
		// GridItemCommandListSelect's cycling "<"/">" in place of one).
		auto needle = m_SearchBuffer;
		std::transform(needle.begin(), needle.end(), needle.begin(), [](unsigned char c) {
			return static_cast<char>(std::tolower(c));
		});

		for (size_t i = 0; i < rows.size(); ++i)
		{
			if (needle.empty())
			{
				result.push_back({i, rows[i]});
				continue;
			}

			auto haystack = rows[i];
			std::transform(haystack.begin(), haystack.end(), haystack.begin(), [](unsigned char c) {
				return static_cast<char>(std::tolower(c));
			});

			if (haystack.find(needle) != std::string::npos)
				result.push_back({i, rows[i]});
		}

		return result;
	}

	void GridItemSelectList::draw()
	{
		// Focused/highlighted-only fill, nothing otherwise - same
		// reasoning as GridItemFolder.cpp's identical comment: real
		// Stand's own row rendering only ever draws one rect for
		// whichever row currently has the cursor, never a second fill
		// for every other row just for being present.
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, Theme::kContentItemHeight, Theme::kAccent);

		auto filtered = FilteredRows();
		if (filtered.empty())
			return;

		const auto rowCount = std::min<size_t>(filtered.size(), static_cast<size_t>(m_MaxRows));
		for (size_t i = 0; i < rowCount; ++i)
		{
			if (i != m_HighlightedIndex)
				continue;

			const auto rowY = y + Theme::kContentItemHeight * static_cast<float>(i + 1);
			GridRenderer::DrawRect(x, rowY, width, Theme::kContentItemHeight, Theme::kAccent);
		}
	}

	void GridItemSelectList::drawText()
	{
		const auto displaySearch = m_Editing ? (m_SearchBuffer + "_") : (m_SearchBuffer.empty() ? m_Placeholder : m_SearchBuffer);
		const auto searchColor = (!m_Editing && m_SearchBuffer.empty()) ? Theme::kPlaceholderText : Theme::kText;
		const auto searchSize = GridRenderer::MeasureText(displaySearch.c_str());
		GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (Theme::kContentItemHeight - searchSize.y) * 0.5f), displaySearch.c_str(), searchColor);

		auto filtered = FilteredRows();
		if (filtered.empty())
		{
			const auto noResultsSize = GridRenderer::MeasureText("No results");
			const auto textY = y + Theme::kContentItemHeight + std::max(0.f, (Theme::kContentItemHeight - noResultsSize.y) * 0.5f);
			GridRenderer::DrawText(x + 5.f, textY, "No results", Theme::kPlaceholderText);
			return;
		}

		const auto rowCount = std::min<size_t>(filtered.size(), static_cast<size_t>(m_MaxRows));
		for (size_t i = 0; i < rowCount; ++i)
		{
			const auto rowY = y + Theme::kContentItemHeight * static_cast<float>(i + 1);
			const auto textSize = GridRenderer::MeasureText(filtered[i].value.c_str());
			const auto textY = rowY + std::max(0.f, (Theme::kContentItemHeight - textSize.y) * 0.5f);
			GridRenderer::DrawText(x + 5.f, textY, filtered[i].value.c_str(), Theme::kText);
		}
	}

	void GridItemSelectList::onClickEx(int16_t /*cursorX*/, int16_t cursorY, bool ctrl, bool shift, bool doubleClick)
	{
		const auto rowIndex = static_cast<int>((cursorY - y) / Theme::kContentItemHeight);
		if (rowIndex == 0)
		{
			m_Editing = true;
			return;
		}

		auto filtered = FilteredRows();
		const auto index = static_cast<size_t>(rowIndex - 1);
		if (index < filtered.size() && index < static_cast<size_t>(m_MaxRows))
			Select(filtered[index], ctrl, shift, doubleClick);
	}

	void GridItemSelectList::Select(const FilteredRow& row, bool ctrl, bool shift, bool doubleClick)
	{
		// Re-finds row within a freshly-recomputed filtered list rather
		// than trusting an index handed in from an earlier call - see
		// the class comment for why nothing here is cached across calls.
		auto filtered = FilteredRows();
		for (size_t i = 0; i < filtered.size(); ++i)
		{
			if (filtered[i].index == row.index)
			{
				m_HighlightedIndex = i;
				break;
			}
		}

		if (m_OnSelect)
			m_OnSelect(row.index, row.value, ctrl, shift, doubleClick);
	}

	void GridItemSelectList::activate()
	{
		m_Editing = true;
	}

	bool GridItemSelectList::onArrow(int delta)
	{
		auto filtered = FilteredRows();
		if (filtered.empty())
			return false;

		const auto count = static_cast<int>(filtered.size());
		const auto current = (m_HighlightedIndex < filtered.size()) ? static_cast<int>(m_HighlightedIndex) : -1;
		const auto next = std::clamp(current + (delta > 0 ? 1 : -1), 0, count - 1);

		m_HighlightedIndex = static_cast<size_t>(next);
		if (m_OnSelect)
		{
			// ctrl reads live Left Ctrl state (same GetKeyState() pattern
			// MenuGrid's own sidebar navigation uses) so a consumer that
			// needs a keyboard equivalent of Ctrl+Click (World > Spawn
			// Ped's "hold Ctrl to set player model" instead of spawning)
			// has one - Left Ctrl specifically, distinct from MenuGrid's
			// own Right Ctrl/Shift sidebar-navigation chord. shift/
			// doubleClick stay false: there's no keyboard equivalent for
			// a double-click, and changing shift here would also affect
			// every other GridItemSelectList consumer's own shift-gated
			// behaviour (e.g. Teleport > Saved's delete) as an
			// unrelated side effect.
			const bool ctrl = (GetKeyState(VK_LCONTROL) & 0x8000) != 0;
			m_OnSelect(filtered[static_cast<size_t>(next)].index, filtered[static_cast<size_t>(next)].value, ctrl, false, false);
		}

		return true;
	}

	void GridItemSelectList::onChar(wchar_t c)
	{
		if (!m_Editing)
			return;

		// Printable ASCII only - matches GridItemTextInput's own buffer
		// (see that class's own onChar() comment).
		if (c >= 0x20 && c < 0x7f && m_SearchBuffer.size() < 255)
		{
			m_SearchBuffer.push_back(static_cast<char>(c));

			// The filter just changed out from under it - last call's
			// highlighted row may not even still match.
			m_HighlightedIndex = static_cast<size_t>(-1);
		}
	}

	void GridItemSelectList::onEditKey(unsigned int vkCode)
	{
		if (!m_Editing)
			return;

		switch (vkCode)
		{
		case VK_BACK:
			if (!m_SearchBuffer.empty())
			{
				m_SearchBuffer.pop_back();
				m_HighlightedIndex = static_cast<size_t>(-1);
			}
			break;

		case VK_RETURN:
		case VK_ESCAPE:
			// Neither commits nor discards anything separate from the
			// buffer itself - unlike GridItemTextInput there's no
			// last-committed value to fall back to here. Both just close
			// the search box back up, keeping whatever's typed so far as
			// the live filter.
			m_Editing = false;
			break;

		default:
			break;
		}
	}
}
