#include "Rendering/GridItemPlayerList.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Network/Players.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <utility>
#include <vector>

namespace Stand::Rendering
{
	namespace
	{
		// Player passed by value in the comparator (not const&) - same
		// as MenuPlayers.cpp's own ComparePlayerNames, since Player::
		// GetName()/operator==() aren't const and Player itself is a
		// plain 8-byte handle (see Player.hpp's own static_assert),
		// cheap to copy. For the same reason, every SortedPlayers()
		// caller below keeps its own result in a non-const local (never
		// `const auto players = ...`) - GetName()/operator==() can't be
		// called on a Player reached through a const vector/pair
		// otherwise.
		std::vector<std::pair<uint8_t, Player>> SortedPlayers()
		{
			std::vector<std::pair<uint8_t, Player>> sorted(Players::GetPlayers().begin(), Players::GetPlayers().end());
			std::sort(sorted.begin(), sorted.end(), [](std::pair<uint8_t, Player> a, std::pair<uint8_t, Player> b) {
				return std::string(a.second.GetName()) < std::string(b.second.GetName());
			});
			return sorted;
		}
	}

	GridItemPlayerList::GridItemPlayerList(int16_t width) :
	    GridItem(GRIDITEM_INDIFFERENT, width, static_cast<int16_t>(Theme::kContentItemHeight * kMaxRows))
	{
	}

	void GridItemPlayerList::draw()
	{
		// Selected-only fill, nothing otherwise - see GridItemFolder.cpp's
		// identical comment for why (confirmed against real Stand's own
		// row-rendering source: only the one row with the cursor ever
		// gets a rect, not every row just for being present).
		auto players = SortedPlayers();
		auto selected = Players::GetSelected();

		if (players.empty())
			return;

		const auto rowCount = std::min<size_t>(players.size(), static_cast<size_t>(kMaxRows));
		for (size_t i = 0; i < rowCount; ++i)
		{
			if (!(selected.IsValid() && selected == players[i].second))
				continue;

			const auto rowY = y + Theme::kContentItemHeight * static_cast<float>(i);
			GridRenderer::DrawRect(x, rowY, width, Theme::kContentItemHeight, Theme::kAccent);
		}
	}

	void GridItemPlayerList::drawText()
	{
		auto players = SortedPlayers();

		if (players.empty())
		{
			const auto textY = y + std::max(0.f, (Theme::kContentItemHeight - GridRenderer::MeasureText("No players").y) * 0.5f);
			GridRenderer::DrawText(x + 5.f, textY, "No players", Theme::kPlaceholderText);
			return;
		}

		const auto rowCount = std::min<size_t>(players.size(), static_cast<size_t>(kMaxRows));
		for (size_t i = 0; i < rowCount; ++i)
		{
			const auto rowY = y + Theme::kContentItemHeight * static_cast<float>(i);
			const auto textY = rowY + std::max(0.f, (Theme::kContentItemHeight - GridRenderer::MeasureText(players[i].second.GetName()).y) * 0.5f);
			GridRenderer::DrawText(x + 5.f, textY, players[i].second.GetName(), Theme::kText);
		}
	}

	void GridItemPlayerList::onClick(int16_t, int16_t cursorY)
	{
		auto players = SortedPlayers();
		if (players.empty())
			return;

		const auto index = static_cast<size_t>((cursorY - y) / Theme::kContentItemHeight);
		if (index < players.size() && index < static_cast<size_t>(kMaxRows))
			Players::SetSelected(players[index].first);
	}

	bool GridItemPlayerList::onArrow(int delta)
	{
		auto players = SortedPlayers();
		if (players.empty())
			return false;

		auto selected = Players::GetSelected();

		size_t currentIndex = 0;
		bool found = false;
		if (selected.IsValid())
		{
			for (size_t i = 0; i < players.size(); ++i)
			{
				if (players[i].second == selected)
				{
					currentIndex = i;
					found = true;
					break;
				}
			}
		}

		const auto count = static_cast<int>(players.size());
		const auto nextIndex = found ? std::clamp(static_cast<int>(currentIndex) + (delta > 0 ? 1 : -1), 0, count - 1) : 0;

		Players::SetSelected(players[static_cast<size_t>(nextIndex)].first);
		return true;
	}
}
