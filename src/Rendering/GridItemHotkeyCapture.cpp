#include "Rendering/GridItemHotkeyCapture.hpp"

#include "Config/HotkeySystem.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <format>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kLabelGap = 10.f;

		// Generous - real capture (holding a key or two, briefly) takes
		// well under this; see this class's own header comment for why
		// it exists at all.
		constexpr std::chrono::seconds kCaptureTimeout{5};

		std::string FormatChain(const CommandLink& link)
		{
			std::string text;
			for (auto key : link.m_Chain)
			{
				if (!text.empty())
					text += "+";
				text += g_HotkeySystem.GetHotkeyLabel(key);
			}
			return text;
		}
	}

	GridItemHotkeyCapture::GridItemHotkeyCapture(int16_t width, int16_t height, std::string label, CommandLink* link) :
	    GridItem(GRIDITEM_INDIFFERENT, width, height),
	    m_Label(std::move(label)),
	    m_Link(link)
	{
	}

	void GridItemHotkeyCapture::StartCapturing()
	{
		if (s_ActiveCapture && s_ActiveCapture != this)
			s_ActiveCapture->StopCapturing();

		m_Capturing = true;
		m_CaptureStart = std::chrono::steady_clock::now();
		s_ActiveCapture = this;
		HotkeySystem::SetBeingModifed(true);
	}

	void GridItemHotkeyCapture::StopCapturing()
	{
		m_Capturing = false;

		if (s_ActiveCapture == this)
			s_ActiveCapture = nullptr;

		HotkeySystem::SetBeingModifed(s_ActiveCapture != nullptr);
	}

	void GridItemHotkeyCapture::activate()
	{
		if (m_Capturing)
			StopCapturing();
		else
			StartCapturing();
	}

	void GridItemHotkeyCapture::draw()
	{
		// Auto-stop if focus moved elsewhere, or capture has been armed
		// too long - see this class's own header comment for why both
		// exist. Checked first, before anything below reads m_Capturing.
		if (m_Capturing && (!isKeyboardFocused() || std::chrono::steady_clock::now() - m_CaptureStart > kCaptureTimeout))
			StopCapturing();

		if (isKeyboardFocused())
			GridRenderer::DrawRect(x, y, width, height, Theme::kAccent);

		// Poll for newly-pressed keys every frame this row is armed -
		// mirrors DrawHotkey.cpp's own per-frame CreateHotkey() call
		// while its button was held; see this class's own header
		// comment for why that lives in draw() here instead.
		if (m_Capturing && m_Link)
			g_HotkeySystem.CreateHotkey(m_Link->m_Chain);
	}

	void GridItemHotkeyCapture::drawText()
	{
		const auto labelSize = GridRenderer::MeasureText(m_Label.c_str());
		GridRenderer::DrawText(x + 5.f, y + std::max(0.f, (height - labelSize.y) * 0.5f), m_Label.c_str(), Theme::kText);

		std::string valueText;
		if (m_Link && !m_Link->m_Chain.empty())
			valueText = FormatChain(*m_Link);
		else if (m_Capturing)
			valueText = "Press any button...";
		else
			valueText = "No hotkey assigned";

		const auto valueX = x + 5.f + labelSize.x + kLabelGap;
		const auto valueSize = GridRenderer::MeasureText(valueText.c_str());
		GridRenderer::DrawText(valueX, y + std::max(0.f, (height - valueSize.y) * 0.5f), valueText.c_str(), m_Capturing ? Theme::kAccent : Theme::kText);
	}
}
