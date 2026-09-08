#include "Rendering/MenuPopup.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <windows.h>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kPadding = 20.f;
		constexpr float kButtonWidth = 80.f;
		constexpr float kButtonGap = 12.f;
		constexpr float kMinWidth = 260.f;
	}

	bool MenuPopup::s_Open = false;
	std::string MenuPopup::s_Message;
	std::function<void()> MenuPopup::s_OnYes;
	std::function<void()> MenuPopup::s_OnNo;

	void MenuPopup::Confirm(std::string message, std::function<void()> onYes, std::function<void()> onNo)
	{
		s_Message = std::move(message);
		s_OnYes = std::move(onYes);
		s_OnNo = std::move(onNo);
		s_Open = true;
	}

	bool MenuPopup::IsOpen()
	{
		return s_Open;
	}

	void MenuPopup::Close()
	{
		s_Open = false;
		s_OnYes = nullptr;
		s_OnNo = nullptr;
	}

	MenuPopup::Layout MenuPopup::ComputeLayout()
	{
		Layout layout{};

		const auto messageSize = GridRenderer::MeasureText(s_Message.c_str());
		layout.buttonWidth = kButtonWidth;
		layout.buttonHeight = Theme::kContentItemHeight;

		layout.width = std::max(kMinWidth, messageSize.x + kPadding * 2.f);
		layout.height = messageSize.y + kPadding * 3.f + layout.buttonHeight;
		layout.x = (Theme::kHudWidth - layout.width) * 0.5f;
		layout.y = (Theme::kHudHeight - layout.height) * 0.5f;

		layout.buttonY = layout.y + layout.height - kPadding - layout.buttonHeight;
		layout.yesX = layout.x + layout.width * 0.5f - kButtonGap * 0.5f - layout.buttonWidth;
		layout.noX = layout.x + layout.width * 0.5f + kButtonGap * 0.5f;
		return layout;
	}

	void MenuPopup::Draw()
	{
		if (!s_Open)
			return;

		const auto layout = ComputeLayout();

		// Full-screen dim behind the dialog, same spirit as ImGui's own
		// modal dimming.
		GridRenderer::DrawRect(0.f, 0.f, Theme::kHudWidth, Theme::kHudHeight, DirectX::XMFLOAT4{0.f, 0.f, 0.f, 0.5f});

		GridRenderer::DrawRect(layout.x, layout.y, layout.width, layout.height, Theme::kPanelBackground);
		GridRenderer::DrawRect(layout.yesX, layout.buttonY, layout.buttonWidth, layout.buttonHeight, Theme::kAccent);
		GridRenderer::DrawRect(layout.noX, layout.buttonY, layout.buttonWidth, layout.buttonHeight, Theme::kToggleOff);
	}

	void MenuPopup::DrawText()
	{
		if (!s_Open)
			return;

		const auto layout = ComputeLayout();

		const auto messageSize = GridRenderer::MeasureText(s_Message.c_str());
		GridRenderer::DrawText(layout.x + (layout.width - messageSize.x) * 0.5f, layout.y + kPadding, s_Message.c_str(), Theme::kText);

		const auto yesSize = GridRenderer::MeasureText("Yes");
		GridRenderer::DrawText(layout.yesX + (layout.buttonWidth - yesSize.x) * 0.5f,
		    layout.buttonY + (layout.buttonHeight - yesSize.y) * 0.5f,
		    "Yes",
		    Theme::kText);

		const auto noSize = GridRenderer::MeasureText("No");
		GridRenderer::DrawText(layout.noX + (layout.buttonWidth - noSize.x) * 0.5f,
		    layout.buttonY + (layout.buttonHeight - noSize.y) * 0.5f,
		    "No",
		    Theme::kText);
	}

	void MenuPopup::HandleKey(unsigned int vkCode)
	{
		if (!s_Open)
			return;

		if (vkCode == VK_RETURN)
		{
			auto onYes = std::move(s_OnYes);
			Close();
			if (onYes)
				onYes();
		}
		else if (vkCode == VK_ESCAPE)
		{
			auto onNo = std::move(s_OnNo);
			Close();
			if (onNo)
				onNo();
		}
	}
}
