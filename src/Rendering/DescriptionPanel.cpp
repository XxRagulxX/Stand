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
		constexpr float kPadding   = 8.f;
		constexpr float kLineGap   = 2.f;
		// Gap between separate info boxes — mirrors Stand's own spacer_x
		// (3) between chrome pieces (Grid(default_origin, 3)).
		constexpr float kBoxGap    = 3.f;

		GridItem* FocusedItem()
		{
			auto* current = MenuNavigation::Current();
			if (!current)
				return nullptr;

			return MenuFocus::GetFocusedItem(current);
		}

		// Split description on '\n' and word-wrap each section separately.
		// Each element of the returned vector is one info box's worth of
		// lines — mirrors Stand's own populateCorner() which pushes each
		// piece as a SEPARATE corner vector entry (each → its own
		// GridItemText box). An empty section (two consecutive '\n', or
		// a leading/trailing one) is silently skipped.
		std::vector<std::vector<std::string>> GetSections(
		    const std::string& description, float maxWidth, float scale)
		{
			std::vector<std::vector<std::string>> sections;
			size_t pos = 0;
			while (pos <= description.size())
			{
				const auto next = description.find('\n', pos);
				const auto end  = (next == std::string::npos) ? description.size() : next;
				if (end > pos)
				{
					auto lines = WrapText(description.substr(pos, end - pos), maxWidth, scale);
					if (!lines.empty())
						sections.push_back(std::move(lines));
				}
				if (next == std::string::npos)
					break;
				pos = next + 1;
			}
			return sections;
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
			const float w   = static_cast<float>(Theme::kInfoWidth);

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

		float BoxHeight(const std::vector<std::string>& lines, float lineHeight)
		{
			if (lines.empty())
				return 0.f;
			return kPadding * 2.f
			    + static_cast<float>(lines.size()) * lineHeight
			    + static_cast<float>(lines.size() - 1) * kLineGap;
		}
	}

	void DescriptionPanel::Draw(int16_t contentX, int16_t contentY, int16_t contentRightX, int16_t sidebarX, int16_t sidebarBottomY)
	{
		auto* item = FocusedItem();
		if (!item)
			return;

		const auto description = item->GetDescription();
		if (description.empty())
			return;

		const float scale    = Theme::kSmallTextScaleMutable;
		const float maxWidth = static_cast<float>(Theme::kInfoWidth) - kPadding * 2.f;
		const auto sections  = GetSections(description, maxWidth, scale);
		if (sections.empty())
			return;

		const auto origin     = ComputeOrigin(contentX, contentY, contentRightX, sidebarX, sidebarBottomY);
		const auto lineHeight = GridRenderer::MeasureText("Ag", scale).y;

		float y = origin.y;
		for (const auto& lines : sections)
		{
			const float boxH = BoxHeight(lines, lineHeight);
			GridRenderer::DrawRect(origin.x, y, origin.width, boxH, Theme::kPanelBackground);
			y += boxH + kBoxGap;
		}
	}

	void DescriptionPanel::DrawText(int16_t contentX, int16_t contentY, int16_t contentRightX, int16_t sidebarX, int16_t sidebarBottomY)
	{
		auto* item = FocusedItem();
		if (!item)
			return;

		const auto description = item->GetDescription();
		if (description.empty())
			return;

		const float scale    = Theme::kSmallTextScaleMutable;
		const float maxWidth = static_cast<float>(Theme::kInfoWidth) - kPadding * 2.f;
		const auto sections  = GetSections(description, maxWidth, scale);
		if (sections.empty())
			return;

		const auto origin     = ComputeOrigin(contentX, contentY, contentRightX, sidebarX, sidebarBottomY);
		const auto lineHeight = GridRenderer::MeasureText("Ag", scale).y;
		const float textXBase = origin.x + kPadding + Theme::kSmallTextXOffset;

		float y = origin.y;
		for (const auto& lines : sections)
		{
			float textY = y + kPadding + Theme::kSmallTextYOffset;
			for (const auto& line : lines)
			{
				GridRenderer::DrawText(textXBase, textY, line.c_str(), Theme::kText, scale);
				textY += lineHeight + kLineGap;
			}
			y += BoxHeight(lines, lineHeight) + kBoxGap;
		}
	}
}
