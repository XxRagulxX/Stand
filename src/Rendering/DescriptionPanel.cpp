#include "Rendering/DescriptionPanel.hpp"

#include "Rendering/GridItem.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Rendering/MenuFocus.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/TextWrap.hpp"
#include "Rendering/Theme.hpp"

#include <string>
#include <vector>

namespace Stand::Rendering
{
	namespace
	{
		constexpr float kPadding = 8.f;
		constexpr float kLineGap = 2.f;
		constexpr float kGroupGap = 12.f;

		GridItem* FocusedItem()
		{
			auto* current = MenuNavigation::Current();
			if (!current)
				return nullptr;

			return MenuFocus::GetFocusedItem(current);
		}

		struct ContentGroups
		{
			std::vector<std::string> descLines;
			std::vector<std::string> syntaxLines;
		};

		ContentGroups GetContent()
		{
			auto* item = FocusedItem();
			if (!item)
				return {};

			const auto description = item->GetDescription();
			if (description.empty())
				return {};

			const float maxWidth = static_cast<float>(Theme::kInfoWidth) - kPadding * 2.f;

			ContentGroups groups;
			const auto split = description.find('\n');

			auto wrapInto = [&](const std::string& text, std::vector<std::string>& target) {
				for (auto& line : WrapText(text, maxWidth, Theme::kSmallTextScale))
					target.push_back(std::move(line));
			};

			if (split == std::string::npos)
			{
				if (Theme::kShowHelpText)
					wrapInto(description, groups.descLines);
			}
			else
			{
				if (Theme::kShowHelpText)
					wrapInto(description.substr(0, split), groups.descLines);
				if (Theme::kShowSyntax)
					wrapInto(description.substr(split + 1), groups.syntaxLines);
			}

			return groups;
		}

		struct PanelOrigin
		{
			float x;
			float y;
			float width;
		};

		PanelOrigin ComputeOrigin(int16_t contentX, int16_t contentY, int16_t contentRightX, int16_t sidebarX, int16_t sidebarBottomY)
		{
			const float pad = static_cast<float>(Theme::kInfoPadding);
			const float w = static_cast<float>(Theme::kInfoWidth);

			switch (Theme::kInfoTextPosition)
			{
			case Theme::InfoTextPosition::Right:
				return {static_cast<float>(contentRightX) + pad, static_cast<float>(contentY), w};
			case Theme::InfoTextPosition::Bottom:
				return {static_cast<float>(contentX), static_cast<float>(sidebarBottomY) + pad, w};
			case Theme::InfoTextPosition::BelowTabs:
				return {static_cast<float>(sidebarX), static_cast<float>(sidebarBottomY) + pad, w};
			default:
				return {static_cast<float>(contentX) - Theme::kSpacer - w - pad,
				    static_cast<float>(sidebarBottomY > contentY ? sidebarBottomY : contentY), w};
			}
		}

		float TotalHeight(const ContentGroups& groups, float lineHeight)
		{
			const auto descCount = static_cast<int>(groups.descLines.size());
			const auto syntaxCount = static_cast<int>(groups.syntaxLines.size());
			const auto totalLines = descCount + syntaxCount;
			if (totalLines == 0)
				return 0.f;

			float h = kPadding * 2.f
			    + static_cast<float>(totalLines) * lineHeight
			    + static_cast<float>(totalLines - 1) * kLineGap;

			if (descCount > 0 && syntaxCount > 0)
				h += kGroupGap;

			return h;
		}
	}

	void DescriptionPanel::Draw(int16_t contentX, int16_t contentY, int16_t contentRightX, int16_t sidebarX, int16_t sidebarBottomY)
	{
		const auto groups = GetContent();
		if (groups.descLines.empty() && groups.syntaxLines.empty())
			return;

		const auto origin = ComputeOrigin(contentX, contentY, contentRightX, sidebarX, sidebarBottomY);
		const auto lineHeight = GridRenderer::MeasureText("Ag", Theme::kSmallTextScale).y;
		GridRenderer::DrawRect(origin.x, origin.y, origin.width, TotalHeight(groups, lineHeight), Theme::kPanelBackground);
	}

	void DescriptionPanel::DrawText(int16_t contentX, int16_t contentY, int16_t contentRightX, int16_t sidebarX, int16_t sidebarBottomY)
	{
		const auto groups = GetContent();
		if (groups.descLines.empty() && groups.syntaxLines.empty())
			return;

		const auto origin = ComputeOrigin(contentX, contentY, contentRightX, sidebarX, sidebarBottomY);
		const auto lineHeight = GridRenderer::MeasureText("Ag", Theme::kSmallTextScale).y;
		const float x = origin.x + kPadding;
		float y = origin.y + kPadding;

		for (const auto& line : groups.descLines)
		{
			GridRenderer::DrawText(x, y, line.c_str(), Theme::kText, Theme::kSmallTextScale);
			y += lineHeight + kLineGap;
		}

		if (!groups.descLines.empty() && !groups.syntaxLines.empty())
			y += kGroupGap;

		for (const auto& line : groups.syntaxLines)
		{
			GridRenderer::DrawText(x, y, line.c_str(), Theme::kText, Theme::kSmallTextScale);
			y += lineHeight + kLineGap;
		}
	}
}
