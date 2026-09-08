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

		GridItem* FocusedItem()
		{
			auto* current = MenuNavigation::Current();
			if (!current)
				return nullptr;

			return MenuFocus::GetFocusedItem(current);
		}

		// Recomputed fresh on every Draw()/DrawText() call rather than
		// cached - cheap (one virtual call plus a word-wrap over a
		// handful of words), and there's no shared per-frame state to
		// cache into here the way MenuGrid's own populate() has.
		//
		// GetDescription() may embed '\n' as a hard break between
		// distinct logical lines (e.g. GridItemStandCommand's own
		// help_text + "Command: ... [on/off]" - see its own comment) -
		// WrapText()'s own word-splitting (istringstream's `>>`) treats
		// any whitespace, '\n' included, as just another space, so a
		// naive single WrapText() call over the whole description would
		// run those two logical lines together into one paragraph
		// instead of keeping them on their own line(s). Split on '\n'
		// first, word-wrap each resulting segment independently, then
		// concatenate - matches real Stand's own populateCorner(), which
		// builds each corner entry as its own separate GridItemText row
		// rather than one big joined block.
		std::vector<std::string> WrappedLines()
		{
			auto* item = FocusedItem();
			if (!item)
				return {};

			auto description = item->GetDescription();
			if (description.empty())
				return {};

			const auto maxWidth = static_cast<float>(Theme::kInfoWidth) - kPadding * 2.f;

			std::vector<std::string> lines;
			size_t start = 0;
			while (start <= description.size())
			{
				const auto end = description.find('\n', start);
				const auto segment = description.substr(start, end == std::string::npos ? std::string::npos : end - start);

				for (auto& wrapped : WrapText(segment, maxWidth, Theme::kSmallTextScale))
					lines.push_back(std::move(wrapped));

				if (end == std::string::npos)
					break;
				start = end + 1;
			}

			return lines;
		}

		// The box's own top-left corner, in H-space - see this class's
		// own header comment for the setPositions() collision this
		// mirrors: right edge just left of content's own left edge
		// (contentX - spacer - kInfoWidth), y at whichever of content's
		// own top or the sidebar's own bottom edge is lower.
		float OriginX(int16_t contentX)
		{
			return static_cast<float>(contentX) - Theme::kSpacer - static_cast<float>(Theme::kInfoWidth);
		}

		float OriginY(int16_t contentY, int16_t sidebarBottomY)
		{
			return static_cast<float>(sidebarBottomY > contentY ? sidebarBottomY : contentY);
		}
	}

	void DescriptionPanel::Draw(int16_t contentX, int16_t contentY, int16_t sidebarBottomY)
	{
		const auto lines = WrappedLines();
		if (lines.empty())
			return;

		const auto lineHeight = GridRenderer::MeasureText("Ag", Theme::kSmallTextScale).y;
		const float height = kPadding * 2.f + static_cast<float>(lines.size()) * lineHeight + static_cast<float>(lines.size() - 1) * kLineGap;

		GridRenderer::DrawRect(OriginX(contentX), OriginY(contentY, sidebarBottomY), static_cast<float>(Theme::kInfoWidth), height, Theme::kPanelBackground);
	}

	void DescriptionPanel::DrawText(int16_t contentX, int16_t contentY, int16_t sidebarBottomY)
	{
		const auto lines = WrappedLines();
		if (lines.empty())
			return;

		const auto lineHeight = GridRenderer::MeasureText("Ag", Theme::kSmallTextScale).y;
		const float x = OriginX(contentX) + kPadding;
		float y = OriginY(contentY, sidebarBottomY) + kPadding;
		for (const auto& line : lines)
		{
			GridRenderer::DrawText(x, y, line.c_str(), Theme::kText, Theme::kSmallTextScale);
			y += lineHeight + kLineGap;
		}
	}
}
