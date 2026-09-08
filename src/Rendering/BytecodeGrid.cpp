#include "Rendering/BytecodeGrid.hpp"

#include "Rendering/GridItemButton.hpp"
#include "Rendering/GridItemHexRow.hpp"
#include "Rendering/GridItemText.hpp"
#include "Rendering/GridItemTextInput.hpp"
#include "Rendering/MenuFocus.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/scrProgram.hpp"

#include <cstdlib>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kSectionHeaderH = Theme::kContentItemHeight;
		constexpr float kItemH = Theme::kContentItemHeight;
	}

	// Origin (1438, 587) matches every other content Grid's. Spacer is
	// 0, not 3 - confirmed against real Stand's own source (origin/
	// stand-reference) that individual list rows have zero gap between
	// them; the 3-unit spacer real Stand does use is only ever between
	// distinct chrome pieces (addressbar/tabs/list), never between rows -
	// see the comment in MenuGrid.cpp's anonymous namespace for why (no
	// shared header for these yet). Reached through ThreadsGrid's own
	// "Bytecode" folder row, a plain GridItemFolder navigation
	// (MenuNavigation replaces the whole content column) rather than a
	// GridTabbed target, so it uses the normal origin - not the offset
	// one a GridTabbed target needs to clear its own tab strip (see
	// GridTabbed.hpp's own class comment).
	BytecodeGrid::BytecodeGrid() :
	    Grid(Theme::GetContentOrigin(), 0)
	{
	}

	void BytecodeGrid::SetProgram(rage::scrProgram* program)
	{
		if (m_Program == program)
			return;

		m_Program = program;
		invalidate();
	}

	void BytecodeGrid::populate(std::vector<std::unique_ptr<GridItem>>& items_draft)
	{
		m_Rows.clear();

		if (!m_Program)
		{
			items_draft.push_back(std::make_unique<GridItemText>(Theme::kContentWidth, kSectionHeaderH, "No thread selected", Theme::kError));
			return;
		}

		auto offsetInput = std::make_unique<GridItemTextInput>(Theme::kContentWidth, kItemH, "Jump to Offset", "", nullptr);
		auto* offsetInputPtr = offsetInput.get();
		items_draft.push_back(std::move(offsetInput));

		items_draft.push_back(std::make_unique<GridItemButton>(Theme::kContentWidth, kItemH, "Jump", [this, offsetInputPtr] {
			if (!m_Program)
				return;

			const std::string text = offsetInputPtr->GetValue();
			char* end = nullptr;
			const std::uint32_t offset = std::strtoul(text.c_str(), &end, 0); // base 0 - accepts "0x.."/decimal, same as the original's own strtoul()

			if (end == text.c_str() || offset >= m_Program->GetFullCodeSize())
				return;

			const auto row = offset / 16;
			if (row >= m_Rows.size())
				return;

			MenuFocus::SetFocusedItem(this, m_Rows[row]);

			const auto visibleHeight = static_cast<int16_t>(Theme::kHudHeight - origin.y - Theme::kContentBottomMargin);
			ScrollToShow(m_Rows[row], visibleHeight);
		}));

		const std::uint32_t codeSize = m_Program->GetFullCodeSize();
		const std::uint32_t totalRows = (codeSize + 15) / 16;

		m_Rows.reserve(totalRows);
		for (std::uint32_t row = 0; row < totalRows; row++)
		{
			auto hexRow = std::make_unique<GridItemHexRow>(Theme::kContentWidth, kItemH, m_Program, row * 16, codeSize);
			m_Rows.push_back(hexRow.get());
			items_draft.push_back(std::move(hexRow));
		}
	}
}
