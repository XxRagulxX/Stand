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
		// Extra vertical space inserted between the description paragraph
		// and the command syntax line - matches real Stand's own corner panel
		// where help_text and getCommandSyntax() appear as two visually
		// distinct stacked groups, not a single run-together block.
		constexpr float kGroupGap = 12.f;

		GridItem* FocusedItem()
		{
			auto* current = MenuNavigation::Current();
			if (!current)
				return nullptr;

			return MenuFocus::GetFocusedItem(current);
		}

		// Two-group content: the description paragraph (help_text, word-
		// wrapped) and the command syntax line ("Command: name [on/off]",
		// word-wrapped). Separated by the single '\n' GetDescription()
		// embeds between them - everything before the first '\n' is the
		// description group, everything after is the syntax group.
		// Either group may be empty (a command with no help text has only
		// a syntax group; a list/action with no syntax has only a desc
		// group; a command with neither returns both empty).
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
				wrapInto(description, groups.descLines);
			}
			else
			{
				wrapInto(description.substr(0, split), groups.descLines);
				wrapInto(description.substr(split + 1), groups.syntaxLines);
			}

			return groups;
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

			// Extra inter-group gap when both groups are present.
			if (descCount > 0 && syntaxCount > 0)
				h += kGroupGap;

			return h;
		}
	}

	void DescriptionPanel::Draw(int16_t contentX, int16_t contentY, int16_t sidebarBottomY)
	{
		const auto groups = GetContent();
		if (groups.descLines.empty() && groups.syntaxLines.empty())
			return;

		const auto lineHeight = GridRenderer::MeasureText("Ag", Theme::kSmallTextScale).y;
		GridRenderer::DrawRect(OriginX(contentX), OriginY(contentY, sidebarBottomY),
		    static_cast<float>(Theme::kInfoWidth), TotalHeight(groups, lineHeight), Theme::kPanelBackground);
	}

	void DescriptionPanel::DrawText(int16_t contentX, int16_t contentY, int16_t sidebarBottomY)
	{
		const auto groups = GetContent();
		if (groups.descLines.empty() && groups.syntaxLines.empty())
			return;

		const auto lineHeight = GridRenderer::MeasureText("Ag", Theme::kSmallTextScale).y;
		const float x = OriginX(contentX) + kPadding;
		float y = OriginY(contentY, sidebarBottomY) + kPadding;

		// Description paragraph - normal text color.
		for (const auto& line : groups.descLines)
		{
			GridRenderer::DrawText(x, y, line.c_str(), Theme::kText, Theme::kSmallTextScale);
			y += lineHeight + kLineGap;
		}

		// Extra gap before the syntax group when both are present -
		// matches real Stand's visual: the "Command: name [on/off]" line
		// sits clearly separate from the help_text paragraph above it.
		if (!groups.descLines.empty() && !groups.syntaxLines.empty())
			y += kGroupGap;

		// Command syntax line(s) - placeholder/dimmer color, matching
		// real Stand's own corner where the syntax entry is visually
		// subordinate to the description text above it.
		for (const auto& line : groups.syntaxLines)
		{
			GridRenderer::DrawText(x, y, line.c_str(), Theme::kText, Theme::kSmallTextScale);
			y += lineHeight + kLineGap;
		}
	}
}
