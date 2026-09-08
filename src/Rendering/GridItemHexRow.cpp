#include "Rendering/GridItemHexRow.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/scrProgram.hpp"

#include <algorithm>
#include <cstdlib>
#include <format>
#include <windows.h>

namespace Stand::Rendering
{
	namespace
	{
		// Sized to fit Theme::kContentWidth (450) - 60 + 16 * (20 + 2) = 412.
		constexpr float kOffsetLabelWidth = 60.f;
		constexpr float kCellWidth = 20.f;
		constexpr float kCellGap = 2.f;
		constexpr int kBytesPerRow = 16;
	}

	GridItemHexRow::GridItemHexRow(int16_t width, int16_t height, rage::scrProgram* program, std::uint32_t rowOffset, std::uint32_t codeSize) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Program(program),
	    m_RowOffset(rowOffset),
	    m_CodeSize(codeSize)
	{
	}

	GridItemHexRow::Layout GridItemHexRow::ComputeLayout() const
	{
		Layout layout;
		layout.offsetLabelWidth = kOffsetLabelWidth;
		layout.cellWidth = kCellWidth;
		layout.cellGap = kCellGap;
		layout.firstCellX = x + layout.offsetLabelWidth;
		return layout;
	}

	int GridItemHexRow::CellCount() const
	{
		if (m_RowOffset >= m_CodeSize)
			return 0;

		return static_cast<int>(std::min<std::uint32_t>(kBytesPerRow, m_CodeSize - m_RowOffset));
	}

	void GridItemHexRow::draw()
	{
		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);

		const auto layout = ComputeLayout();
		const int cellCount = CellCount();

		for (int i = 0; i < cellCount; i++)
		{
			const float cellX = layout.firstCellX + i * (layout.cellWidth + layout.cellGap);

			// The selected cell gets an accent box of its own (on top of
			// the row's own highlight, if focused); every other cell gets
			// no fill at all - see GridItemFolder.cpp's identical comment
			// for why (confirmed against real Stand's own row-rendering
			// source).
			if (isKeyboardFocused() && i == m_SelectedCell)
				GridRenderer::DrawRect(cellX, y, layout.cellWidth, height, Theme::kAccent);
		}
	}

	void GridItemHexRow::drawText()
	{
		const auto layout = ComputeLayout();
		const int cellCount = CellCount();

		const auto offsetStr = std::format("{:08X}:", m_RowOffset);
		const auto offsetSize = GridRenderer::MeasureText(offsetStr.c_str());
		GridRenderer::DrawText(x, y + std::max(0.f, (height - offsetSize.y) * 0.5f), offsetStr.c_str(), Theme::kText);

		for (int i = 0; i < cellCount; i++)
		{
			const float cellX = layout.firstCellX + i * (layout.cellWidth + layout.cellGap);

			std::string cellText;
			if (m_Editing && i == m_SelectedCell)
			{
				cellText = m_EditBuffer + "_";
			}
			else if (auto* byte = m_Program ? m_Program->GetCodeAddress(m_RowOffset + i) : nullptr)
			{
				cellText = std::format("{:02X}", static_cast<unsigned int>(*byte));
			}
			else
			{
				cellText = "??";
			}

			const auto cellSize = GridRenderer::MeasureText(cellText.c_str());
			GridRenderer::DrawText(cellX + std::max(0.f, (layout.cellWidth - cellSize.x) * 0.5f),
			    y + std::max(0.f, (height - cellSize.y) * 0.5f),
			    cellText.c_str(),
			    Theme::kText);
		}
	}

	void GridItemHexRow::onClick(int16_t cursorX, int16_t)
	{
		const auto layout = ComputeLayout();
		const int cellCount = CellCount();

		for (int i = 0; i < cellCount; i++)
		{
			const float cellX = layout.firstCellX + i * (layout.cellWidth + layout.cellGap);
			if (cursorX >= cellX && cursorX < cellX + layout.cellWidth)
			{
				m_SelectedCell = i;
				return;
			}
		}
	}

	bool GridItemHexRow::onArrow(int delta)
	{
		const int cellCount = CellCount();
		if (cellCount == 0)
			return false;

		m_SelectedCell = std::clamp(m_SelectedCell + (delta > 0 ? 1 : -1), 0, cellCount - 1);
		return true;
	}

	void GridItemHexRow::activate()
	{
		BeginEditing();
	}

	void GridItemHexRow::BeginEditing()
	{
		// A no-op if already editing - same guard as GridItemTextInput's
		// own BeginEditing(), and for the same reason: re-pressing Enter
		// mid-edit would otherwise reset m_EditBuffer back to the live
		// byte's current text, discarding whatever's been typed since.
		if (m_Editing)
			return;

		if (CellCount() == 0)
			return;

		auto* byte = m_Program ? m_Program->GetCodeAddress(m_RowOffset + m_SelectedCell) : nullptr;
		m_EditBuffer = byte ? std::format("{:02X}", static_cast<unsigned int>(*byte)) : "";
		m_Editing = true;
	}

	void GridItemHexRow::Commit()
	{
		m_Editing = false;

		auto* byte = m_Program ? m_Program->GetCodeAddress(m_RowOffset + m_SelectedCell) : nullptr;
		if (!byte || m_EditBuffer.empty())
			return;

		char* end = nullptr;
		const auto newValue = static_cast<std::uint8_t>(std::strtoul(m_EditBuffer.c_str(), &end, 16));
		if (end != m_EditBuffer.c_str())
			*byte = newValue; // see this class's own header comment - unchanged from the original's own direct write
	}

	void GridItemHexRow::Cancel()
	{
		m_Editing = false;
	}

	void GridItemHexRow::onChar(wchar_t c)
	{
		if (!m_Editing)
			return;

		// Hex digits only, at most 2 characters (one byte) - matches the
		// original's own ImGuiInputTextFlags_CharsHexadecimal plus its
		// 3-byte (2 hex chars + nul) hexStr buffer.
		const bool isHexDigit = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
		if (isHexDigit && m_EditBuffer.size() < 2)
			m_EditBuffer.push_back(static_cast<char>(c));
	}

	void GridItemHexRow::onEditKey(unsigned int vkCode)
	{
		if (!m_Editing)
			return;

		switch (vkCode)
		{
		case VK_BACK:
			if (!m_EditBuffer.empty())
				m_EditBuffer.pop_back();
			break;

		case VK_RETURN:
			Commit();
			break;

		case VK_ESCAPE:
			Cancel();
			break;

		default:
			break;
		}
	}
}
