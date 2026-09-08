#include "Rendering/GridItemTextInput.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/InputCapture.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <cmath>
#include <windows.h>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kLabelGap = 10.f;
		constexpr float kValuePaddingX = 6.f;
	}

	GridItemTextInput::GridItemTextInput(int16_t width,
	    int16_t height,
	    std::string label,
	    std::string initialValue,
	    std::function<void(const std::string&)> onCommit,
	    std::function<void(const std::string&)> onChange,
	    bool scrolling,
	    std::size_t maxLength,
	    std::function<void()> onLimitReached) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Label(std::move(label)),
	    m_Value(std::move(initialValue)),
	    m_OnCommit(std::move(onCommit)),
	    m_OnChange(std::move(onChange)),
	    m_Scrolling(scrolling),
	    m_MaxLength(maxLength),
	    m_OnLimitReached(std::move(onLimitReached))
	{
	}

	void GridItemTextInput::draw()
	{
		// Auto-commit if focus moved elsewhere while still editing - see
		// the class comment in GridItemTextInput.hpp. Checked here since
		// draw() already runs every frame this item is visible, and
		// there's no separate "blur" event in this system to hook
		// instead.
		if (m_Editing && !isKeyboardFocused())
			Commit();

		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);

		// Value box fills the rest of the row after the label, same
		// left-to-right sequencing GridItemCommandListSelect uses - see that
		// widget's own class comment for why (a value box anchored to
		// this item's own right edge would clip a long label instead).
		const auto labelWidth = GridRenderer::MeasureText(m_Label.c_str()).x;
		const auto valueX = x + 5.f + labelWidth + kLabelGap;
		const auto valueWidth = std::max(0.f, static_cast<float>(width) - (valueX - x));
		GridRenderer::DrawRect(valueX, y, valueWidth, height, Theme::kPanelBackground);
	}

	void GridItemTextInput::drawText()
	{
		const auto labelSize = GridRenderer::MeasureText(m_Label.c_str());
		GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), m_Label.c_str(), Theme::kText);

		// A trailing "_" caret while editing - the simplest possible
		// "you're typing here" indicator without real cursor placement
		// (see the class comment in the header for why there isn't one).
		const auto displayValue = m_Editing ? (m_Buffer + "_") : m_Value;
		const auto valueSize = GridRenderer::MeasureText(displayValue.c_str());
		const auto labelWidth = GridRenderer::MeasureText(m_Label.c_str()).x;
		const auto valueX = x + 5.f + labelWidth + kLabelGap;
		const auto valueY = y + std::max(0.f, (height - valueSize.y) * 0.5f);

		if (!m_Editing && m_Scrolling)
		{
			const auto valueWidth = std::max(0.f, static_cast<float>(width) - (valueX - x));
			if (valueSize.x > valueWidth)
			{
				constexpr float kGapPx = 40.f;
				constexpr float kPixelsPerMs = 0.03f;
				const auto cycle = valueSize.x + kGapPx;
				const auto offset = std::fmod(static_cast<float>(GetTickCount64()) * kPixelsPerMs, cycle);
				GridRenderer::DrawText(valueX + kValuePaddingX - offset, valueY, displayValue.c_str(), Theme::kText);
				GridRenderer::DrawText(valueX + kValuePaddingX - offset + cycle, valueY, displayValue.c_str(), Theme::kText);
				return;
			}
		}

		GridRenderer::DrawText(valueX + kValuePaddingX, valueY, displayValue.c_str(), Theme::kText);
	}

	void GridItemTextInput::onClick(int16_t, int16_t)
	{
		activate();
	}

	void GridItemTextInput::activate()
	{
		BeginEditing();
	}

	void GridItemTextInput::BeginEditing()
	{
		// A no-op if already editing - re-clicking (or pressing Enter
		// on) a field mid-edit would otherwise reset m_Buffer back to
		// the last committed value, discarding whatever's been typed
		// since.
		if (m_Editing)
			return;

		m_Buffer = m_Value;
		m_Editing = true;
		InputCapture::SetTextInputActive(true);
	}

	void GridItemTextInput::Commit()
	{
		m_Value = m_Buffer;
		m_Editing = false;
		InputCapture::SetTextInputActive(false);

		if (m_OnCommit)
			m_OnCommit(m_Value);
	}

	void GridItemTextInput::Cancel()
	{
		m_Editing = false;
		InputCapture::SetTextInputActive(false);
	}

	void GridItemTextInput::onChar(wchar_t c)
	{
		if (!m_Editing)
			return;

		// Printable ASCII only - matches this project's existing
		// StringCommandItem's own 256-byte narrow buffer (src/
		// StringCommandItem.cpp), no wide-character/IME support.
		if (c < 0x20 || c >= 0x7f)
			return;

		// m_MaxLength defaults to the original hardcoded 255 above -
		// see this class's own header comment (ported from real Stand's
		// own CommandInputTextLimitChars) for callers that configure a
		// tighter one.
		if (m_Buffer.size() >= m_MaxLength)
		{
			if (m_OnLimitReached)
				m_OnLimitReached();
			return;
		}

		m_Buffer.push_back(static_cast<char>(c));

		if (m_OnChange)
			m_OnChange(m_Buffer);
	}

	void GridItemTextInput::onEditKey(unsigned int vkCode)
	{
		if (!m_Editing)
			return;

		switch (vkCode)
		{
		case VK_BACK:
			if (!m_Buffer.empty())
			{
				m_Buffer.pop_back();

				if (m_OnChange)
					m_OnChange(m_Buffer);
			}
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
