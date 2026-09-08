#include "Rendering/Onboarding.hpp"
#include "Menu/GUI.hpp"
#include "Commands/Commands.hpp"
#include "Commands/CommandToggleLegacy.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/TextWrap.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Joaat.hpp"

#include <shellapi.h>

namespace Stand
{
	namespace
	{
		CommandToggleLegacy _OnboardingComplete{"$onboardingcomplete", "", ""};

		constexpr float kPanelWidth = 700.f;
		constexpr float kPad = 16.f;
		constexpr float kRowH = 28.f;
		constexpr float kGap = 8.f;
		constexpr float kTextScale = Rendering::Theme::kSmallTextScale;
		constexpr float kTitleScale = Rendering::Theme::kTextScale;

		enum class Row
		{
			SessionMode,
			OpenGitHub,
			OpenMatrix,
			Close,
			Count
		};

		int g_FocusedRow = 0;
		int g_SessionMode = 0; // 0 = StandEnhanced-only, 1 = Everyone

		constexpr const char* kHint = "Up/Down: navigate   Left/Right: change   Enter: select";

		constexpr const char* kParagraph1 =
		    "Welcome to StandEnhanced! You can press INSERT or Ctrl+\\ to open the menu. With the introduction of BattlEye, the ability to join and stay in public sessions has been severely limited. "
		    "You have an option to play only with other StandEnhanced users, or you can choose to connect to regular BattlEye-protected sessions. "
		    "You will automatically be kicked out of regular sessions in under three minutes, and you may be temporarily blacklisted from "
		    "joining for up to two days, even after re-enabling BattlEye";

		constexpr const char* kParagraph2 =
		    "Have Fun";

		constexpr const char* kParagraph3 = "Check for updates reguarly; we publish new builds every night. But most importantly, mess around and have fun with StandEnhanced!";

		constexpr const char* kTitle = "IMPORTANT! PLEASE READ!";

		// Every position DrawImpl's rect pass and DrawTextImpl's text
		// pass both need, re-derived identically (and independently) by
		// each - same reasoning as every other overlay ported this
		// session (see e.g. Notifications.cpp's own Layout/ComputeLayout).
		// Panel height (and so its vertical centering) is computed from
		// actual wrapped content rather than the original's fixed
		// 700x500 guess, so nothing here can ever overflow or clip.
		struct Layout
		{
			bool open = false;
			float panelX = 0.f, panelY = 0.f, panelH = 0.f;
			float lineHeight = 0.f;
			float titleY = 0.f, hintY = 0.f;
			float para1Y = 0.f, para2Y = 0.f, para3Y = 0.f;
			std::vector<std::string> para1Lines, para2Lines, para3Lines;
			float sessionModeY = 0.f, githubY = 0.f, matrixY = 0.f, closeY = 0.f;
		};

		Layout ComputeLayout()
		{
			using namespace Rendering;

			Layout layout;
			layout.open = !_OnboardingComplete.GetState();
			if (!layout.open)
				return layout;

			const float contentWidth = kPanelWidth - kPad * 2.f;
			layout.para1Lines = Rendering::WrapText(kParagraph1, contentWidth, kTextScale);
			layout.para2Lines = Rendering::WrapText(kParagraph2, contentWidth, kTextScale);
			layout.para3Lines = Rendering::WrapText(kParagraph3, contentWidth, kTextScale);
			layout.lineHeight = GridRenderer::MeasureText("Ag", kTextScale).y + 2.f;

			float y = kPad;
			layout.titleY = y;
			y += GridRenderer::MeasureText(kTitle, kTitleScale).y + 4.f;

			layout.hintY = y;
			y += GridRenderer::MeasureText(kHint, kTextScale).y + kGap;

			layout.para1Y = y;
			y += layout.para1Lines.size() * layout.lineHeight + kGap;

			layout.sessionModeY = y;
			y += kRowH + kGap;

			layout.para2Y = y;
			y += layout.para2Lines.size() * layout.lineHeight + kGap;

			layout.githubY = y;
			y += kRowH + kGap * 0.5f;

			layout.matrixY = y;
			y += kRowH + kGap;

			layout.para3Y = y;
			y += layout.para3Lines.size() * layout.lineHeight + kGap;

			layout.closeY = y;
			y += kRowH + kPad;

			layout.panelH = y;
			layout.panelX = (Theme::kHudWidth - kPanelWidth) * 0.5f;
			layout.panelY = (Theme::kHudHeight - layout.panelH) * 0.5f;

			return layout;
		}

		void Complete()
		{
			if (auto* cheaterpool = Commands::GetCommand<CommandToggleLegacy>("cheaterpool"_J))
				cheaterpool->SetState(g_SessionMode == 0);

			_OnboardingComplete.SetState(true);
			GUI::SetOnboarding(false);
		}
	}

	void Onboarding::Draw()
	{
		using namespace Rendering;

		// Mirrors the original's own static-lambda-runs-once idiom -
		// fires GUI::SetOnboarding(true) exactly once, the first time
		// this ever draws while incomplete.
		static bool initialized = [] {
			if (!_OnboardingComplete.GetState())
				GUI::SetOnboarding(true);
			return true;
		}();
		(void)initialized;

		const auto layout = ComputeLayout();
		if (!layout.open)
			return;

		GridRenderer::DrawRect(layout.panelX, layout.panelY, kPanelWidth, layout.panelH, DirectX::XMFLOAT4{0.05f, 0.05f, 0.05f, 0.95f});

		auto row = [&](float yOffset, Row which) {
			const bool focused = g_FocusedRow == static_cast<int>(which);
			GridRenderer::DrawRect(layout.panelX + kPad, layout.panelY + yOffset, kPanelWidth - kPad * 2.f, kRowH, focused ? Theme::kAccent : Theme::kPanelBackground);
		};

		row(layout.sessionModeY, Row::SessionMode);
		row(layout.githubY, Row::OpenGitHub);
		row(layout.matrixY, Row::OpenMatrix);
		row(layout.closeY, Row::Close);
	}

	void Onboarding::DrawText()
	{
		using namespace Rendering;

		const auto layout = ComputeLayout();
		if (!layout.open)
			return;

		const float x = layout.panelX + kPad;
		const float rowTextY = 6.f; // vertical centering of a row's own label within kRowH

		GridRenderer::DrawText(x, layout.panelY + layout.titleY, kTitle, Theme::kAccent, kTitleScale);
		GridRenderer::DrawText(x, layout.panelY + layout.hintY, kHint, Theme::kPlaceholderText, kTextScale);

		float y = layout.panelY + layout.para1Y;
		for (auto& line : layout.para1Lines)
		{
			GridRenderer::DrawText(x, y, line.c_str(), Theme::kText, kTextScale);
			y += layout.lineHeight;
		}

		const char* modeLabel = g_SessionMode == 0 ? "Session Mode:  <  Play with StandEnhanced users  >" : "Session Mode:  <  Play with everyone (Broken!)  >";
		GridRenderer::DrawText(x + 4.f, layout.panelY + layout.sessionModeY + rowTextY, modeLabel, Theme::kText, kTextScale);

		y = layout.panelY + layout.para2Y;
		for (auto& line : layout.para2Lines)
		{
			GridRenderer::DrawText(x, y, line.c_str(), Theme::kText, kTextScale);
			y += layout.lineHeight;
		}

		GridRenderer::DrawText(x + 4.f, layout.panelY + layout.githubY + rowTextY, "Open GitHub", Theme::kText, kTextScale);
		GridRenderer::DrawText(x + 4.f, layout.panelY + layout.matrixY + rowTextY, "Open Matrix Server", Theme::kText, kTextScale);

		y = layout.panelY + layout.para3Y;
		for (auto& line : layout.para3Lines)
		{
			GridRenderer::DrawText(x, y, line.c_str(), Theme::kText, kTextScale);
			y += layout.lineHeight;
		}

		GridRenderer::DrawText(x + 4.f, layout.panelY + layout.closeY + rowTextY, "Close", Theme::kText, kTextScale);
	}

	void Onboarding::HandleKey(unsigned int vkCode)
	{
		if (_OnboardingComplete.GetState())
			return;

		constexpr int rowCount = static_cast<int>(Row::Count);

		switch (vkCode)
		{
		case VK_UP:
			g_FocusedRow = (g_FocusedRow + rowCount - 1) % rowCount;
			break;
		case VK_DOWN:
			g_FocusedRow = (g_FocusedRow + 1) % rowCount;
			break;
		case VK_LEFT:
		case VK_RIGHT:
			if (static_cast<Row>(g_FocusedRow) == Row::SessionMode)
				g_SessionMode = 1 - g_SessionMode;
			break;
		case VK_RETURN:
			switch (static_cast<Row>(g_FocusedRow))
			{
			case Row::OpenGitHub:
				ShellExecuteA(nullptr, "open", "https://github.com/", nullptr, nullptr, SW_SHOWNORMAL);
				break;
			case Row::OpenMatrix:
				ShellExecuteA(nullptr, "open", "https://matrix.to/", nullptr, nullptr, SW_SHOWNORMAL);
				break;
			case Row::Close:
				Complete();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}
