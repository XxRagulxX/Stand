#include "Rendering/MenuCommandBox.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/InputCapture.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <windows.h>

namespace Stand::Rendering
{
	namespace
	{
		// width/x/y match real Stand's own CommandboxGrid exactly
		// (Grid((HUD_WIDTH / 2) - (width / 2), 100, spacer_size), width =
		// COLOURS_WIDTH(11) = 547 - see origin/stand-reference's src/
		// Commands/Widgets/CommandboxGrid.hpp/.cpp). Row heights below
		// are this project's own choice (real Stand's own rows are driven
		// by a different, much larger row set - see this file's class
		// comment) rather than a byte-for-byte match.
		constexpr float kWidth = 547.f;
		constexpr float kY = 100.f;
		constexpr float kTitleHeight = 24.f;
		constexpr float kInputHeight = 32.f;
		constexpr float kLabelHeight = 22.f;
		constexpr float kRangeHeight = 20.f;
		constexpr float kErrorHeight = 20.f;
		constexpr float kPaddingX = 6.f;
		constexpr unsigned long long kErrorFlashMs = 2000;
	}

	bool MenuCommandBox::s_Open = false;
	std::string MenuCommandBox::s_CommandName;
	std::string MenuCommandBox::s_Label;
	std::string MenuCommandBox::s_RangeText;
	std::string MenuCommandBox::s_Buffer;
	std::function<bool(const std::string&)> MenuCommandBox::s_OnSubmit;
	bool MenuCommandBox::s_ShowError = false;
	unsigned long long MenuCommandBox::s_ErrorShownAtMs = 0;

	void MenuCommandBox::Open(std::string commandName, std::string label, std::string rangeText, std::string initialValue, std::function<bool(const std::string&)> onSubmit)
	{
		s_CommandName = std::move(commandName);
		s_Label = std::move(label);
		s_RangeText = std::move(rangeText);
		s_Buffer = s_CommandName + " " + initialValue;
		s_OnSubmit = std::move(onSubmit);
		s_ShowError = false;
		s_Open = true;
		InputCapture::SetTextInputActive(true);
	}

	bool MenuCommandBox::IsOpen()
	{
		return s_Open;
	}

	void MenuCommandBox::Close()
	{
		s_Open = false;
		s_ShowError = false;
		s_OnSubmit = nullptr;
		InputCapture::SetTextInputActive(false);
	}

	MenuCommandBox::Layout MenuCommandBox::ComputeLayout()
	{
		Layout layout{};
		layout.width = kWidth;
		layout.x = (Theme::kHudWidth - kWidth) * 0.5f;
		layout.y = kY;

		layout.titleHeight = kTitleHeight;
		layout.titleY = layout.y;

		layout.inputHeight = kInputHeight;
		layout.inputY = layout.titleY + layout.titleHeight;

		layout.labelHeight = kLabelHeight;
		layout.labelY = layout.inputY + layout.inputHeight;

		layout.rangeHeight = s_RangeText.empty() ? 0.f : kRangeHeight;
		layout.rangeY = layout.labelY + layout.labelHeight;

		layout.errorHeight = kErrorHeight;
		layout.errorY = layout.rangeY + layout.rangeHeight;

		return layout;
	}

	void MenuCommandBox::Draw()
	{
		if (!s_Open)
			return;

		if (s_ShowError && (GetTickCount64() - s_ErrorShownAtMs) > kErrorFlashMs)
			s_ShowError = false;

		const auto layout = ComputeLayout();

		GridRenderer::DrawRect(layout.x, layout.titleY, layout.width, layout.titleHeight, Theme::kAccent);
		GridRenderer::DrawRect(layout.x, layout.inputY, layout.width, layout.inputHeight, Theme::kPanelBackground);
	}

	void MenuCommandBox::DrawText()
	{
		if (!s_Open)
			return;

		const auto layout = ComputeLayout();

		const char* title = "Stand CommandLegacy Box";
		const auto titleSize = GridRenderer::MeasureText(title, Theme::kSmallTextScale);
		GridRenderer::DrawText(layout.x + kPaddingX,
		    layout.titleY + std::max(0.f, (layout.titleHeight - titleSize.y) * 0.5f),
		    title,
		    Theme::kText,
		    Theme::kSmallTextScale);

		const auto displayLine = s_Buffer + "|";
		const auto lineSize = GridRenderer::MeasureText(displayLine.c_str());
		GridRenderer::DrawText(layout.x + kPaddingX, layout.inputY + std::max(0.f, (layout.inputHeight - lineSize.y) * 0.5f), displayLine.c_str(), Theme::kText);

		const auto labelSize = GridRenderer::MeasureText(s_Label.c_str(), Theme::kSmallTextScale);
		GridRenderer::DrawText(layout.x + kPaddingX,
		    layout.labelY + std::max(0.f, (layout.labelHeight - labelSize.y) * 0.5f),
		    s_Label.c_str(),
		    Theme::kPlaceholderText,
		    Theme::kSmallTextScale);

		if (!s_RangeText.empty())
		{
			const auto rangeSize = GridRenderer::MeasureText(s_RangeText.c_str(), Theme::kSmallTextScale);
			GridRenderer::DrawText(layout.x + kPaddingX,
			    layout.rangeY + std::max(0.f, (layout.rangeHeight - rangeSize.y) * 0.5f),
			    s_RangeText.c_str(),
			    Theme::kPlaceholderText,
			    Theme::kSmallTextScale);
		}

		if (s_ShowError)
		{
			const char* error = "Invalid value.";
			const auto errorSize = GridRenderer::MeasureText(error, Theme::kSmallTextScale);
			GridRenderer::DrawText(layout.x + kPaddingX,
			    layout.errorY + std::max(0.f, (layout.errorHeight - errorSize.y) * 0.5f),
			    error,
			    Theme::kError,
			    Theme::kSmallTextScale);
		}
	}

	void MenuCommandBox::HandleKey(unsigned int vkCode)
	{
		if (!s_Open)
			return;

		switch (vkCode)
		{
		case VK_RETURN:
		{
			// Strip the leading whitespace-delimited command-name token,
			// same as real Stand's own Gui::parseCommand - whatever's
			// left (trimmed) is the value token the caller's onSubmit
			// actually interprets. The user is free to have edited or
			// deleted that leading token entirely; only its position
			// (before the first space) matters, not its content.
			auto space = s_Buffer.find(' ');
			std::string valueToken = (space == std::string::npos) ? std::string() : s_Buffer.substr(space + 1);

			auto firstNonSpace = valueToken.find_first_not_of(' ');
			valueToken = (firstNonSpace == std::string::npos) ? std::string() : valueToken.substr(firstNonSpace);

			auto onSubmit = s_OnSubmit;
			if (onSubmit && onSubmit(valueToken))
			{
				Close();
			}
			else
			{
				s_ShowError = true;
				s_ErrorShownAtMs = GetTickCount64();
			}
			break;
		}

		case VK_ESCAPE:
			Close();
			break;

		case VK_BACK:
			if (!s_Buffer.empty())
				s_Buffer.pop_back();
			break;

		default:
			break;
		}
	}

	void MenuCommandBox::HandleChar(wchar_t c)
	{
		if (!s_Open)
			return;

		// Printable ASCII only, same restriction GridItemTextInput's own
		// onChar applies (see that file's own comment) - WM_CHAR's
		// control-character values for Enter/Backspace/Escape (13/8/27)
		// are all below 0x20 and never reach here, so there's no double
		// handling against HandleKey() above.
		if (c >= 0x20 && c < 0x7f && s_Buffer.size() < 255)
			s_Buffer.push_back(static_cast<char>(c));
	}
}
