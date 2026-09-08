#include "Rendering/Grid.hpp"

#include "Commands/CommandToggleLegacy.hpp"
#include "Commands/Commands.hpp"
#include "Rendering/Theme.hpp"

#include <algorithm>
#include <climits>

namespace Stand::Rendering
{
	void Grid::ensurePopulated()
	{
		if (items)
			return;

		// Forgotten and re-registered from scratch every time populate()
		// runs - see watchCondition()'s own doc comment in Grid.hpp.
		m_Watches.clear();
		m_WatchLastValues.clear();

		auto itemsDraft = soup::make_shared<std::vector<std::unique_ptr<GridItem>>>();
		populate(*itemsDraft);
		setPositions(*itemsDraft);
		items = std::move(itemsDraft);
	}

	void Grid::invalidate()
	{
		// soup::SharedPtr has no operator=(nullptr) - reset() is its own
		// equivalent (see stand-reference's src/lib/soup/SharedPtr.hpp).
		items.reset();
		m_ScrollOffset = 0;
	}

	bool Grid::watchCondition(joaat_t hash, bool negate)
	{
		auto* cmd = Commands::GetCommand<CommandToggleLegacy>(hash);
		auto conditionFn = [cmd] {
			return cmd && cmd->GetState();
		};
		return watchCondition(conditionFn, negate);
	}

	bool Grid::watchCondition(std::function<bool()> conditionFn, bool negate)
	{
		if (negate)
		{
			auto positiveFn = std::move(conditionFn);
			conditionFn = [positiveFn] {
				return !positiveFn();
			};
		}

		const bool value = conditionFn();
		m_Watches.push_back(conditionFn);
		m_WatchLastValues.push_back(value);
		return value;
	}

	void Grid::checkWatchedConditions()
	{
		if (!items)
			return;

		for (size_t i = 0; i < m_Watches.size(); ++i)
		{
			if (m_Watches[i]() != m_WatchLastValues[i])
			{
				invalidate();
				return;
			}
		}
	}

	// Ported verbatim from Stand's own Grid::setPositions() (Menu/Grid.cpp).
	void Grid::setPositions(const std::vector<std::unique_ptr<GridItem>>& itemsToPosition, size_t recursions) const
	{
		GridItem* last_item = nullptr;
		{
			auto x = origin.x;
			auto y = origin.y;
			for (auto& item : itemsToPosition)
			{
				if (item->keep_pos)
				{
					item->keep_pos = false;
					x = item->x;
					y = item->y;
				}
				else
				{
					if (item->force_alignment_to != nullptr)
					{
						last_item = item->force_alignment_to;
						x = last_item->x;
						y = last_item->y;
					}
					if (last_item == nullptr)
					{
						switch (item->alignment_relative_to_last)
						{
						case ALIGN_TOP_CENTRE:
							y -= item->height;
							break;
						default:
							break;
						}
					}
					else
					{
						switch (item->alignment_relative_to_last)
						{
						case ALIGN_BOTTOM_LEFT:
							y += (last_item->height + spacer_size);
							break;

						case ALIGN_TOP_RIGHT:
							x += (last_item->width + spacer_size);
							break;

						case ALIGN_TOP_LEFT:
							x -= (item->width + spacer_size);
							if (auto* conflict_item = getOccupantNarrow(itemsToPosition, static_cast<int16_t>(x + item->width - 1), static_cast<int16_t>(y + 1), item.get()))
							{
								y += (conflict_item->height + spacer_size);
							}
							break;

						case ALIGN_BOTTOM_RIGHT:
							x -= (item->width - last_item->width);
							break;

						case ALIGN_TOP_CENTRE:
							y -= (item->height + spacer_size);
							break;

						default:
							break;
						}
					}
					item->x = x;
					item->y = y;
					item->onPositioned();
				}
				last_item = item.get();
			}
		}
		last_item = nullptr;
		bool need_redo = false;
		for (auto& item : itemsToPosition)
		{
			if (item->force_alignment_to != nullptr)
			{
				last_item = item->force_alignment_to;
			}
			switch (item->alignment_relative_to_last)
			{
			case ALIGN_BOTTOM_LEFT:
				if (auto* conflict_item = getOccupant(itemsToPosition, static_cast<int16_t>(item->x + item->width - 1), static_cast<int16_t>(item->y + 1), item.get()))
				{
					item->x = static_cast<int16_t>(item->x - (((item->x + item->width) - conflict_item->x) + spacer_size));
					item->keep_pos = true;
					need_redo = true;
				}
				break;

			default:
				break;
			}
			last_item = item.get();
		}
		if (need_redo && ++recursions <= itemsToPosition.size())
		{
			setPositions(itemsToPosition, recursions);
		}
	}

	GridItem* Grid::getOccupant(const std::vector<std::unique_ptr<GridItem>>& items, int16_t x, int16_t y)
	{
		for (const auto& item : items)
		{
			if (item->occupies(x, y))
				return item.get();
		}
		return nullptr;
	}

	GridItem* Grid::getOccupant(const std::vector<std::unique_ptr<GridItem>>& items, int16_t x, int16_t y, GridItem* exclude)
	{
		for (const auto& item : items)
		{
			if (item.get() != exclude && item->occupies(x, y))
				return item.get();
		}
		return nullptr;
	}

	GridItem* Grid::getOccupantNarrow(const std::vector<std::unique_ptr<GridItem>>& items, int16_t x, int16_t y, GridItem* stop_at)
	{
		for (const auto& item : items)
		{
			if (item.get() == stop_at)
				break;

			if (item->occupies(x, y))
				return item.get();
		}
		return nullptr;
	}

	GridItem* Grid::getItemByType(GridItemType target) const
	{
		return getItemByType(*items, target);
	}

	GridItem* Grid::getItemByType(const std::vector<std::unique_ptr<GridItem>>& items, GridItemType target)
	{
		for (const auto& item : items)
		{
			if (item->type == target)
				return item.get();
		}
		return nullptr;
	}

	// draw()/drawText()/findItemAt() all funnel through this: temporarily
	// shift item->y by -m_ScrollOffset, run fn on it, then restore -
	// applying the current scroll position (see ScrollBy()'s own doc
	// comment in Grid.hpp) without any widget itself needing to know
	// scrolling exists. Items that would scroll above origin.y (this
	// Grid's own top edge - where whatever sits above it, e.g. MenuGrid's
	// own header bar for a content Grid, starts) are skipped outright
	// rather than partially drawn/hit-tested there - this project has no
	// scissor-clipping to partially cut one off at that edge the way
	// Stand's own GridItemList does (see its drawOnto()'d contentsTex),
	// so a hard cut is the trade-off. Nothing does the equivalent for the
	// bottom edge: a row scrolled past the visible window's bottom just
	// runs off the real screen, which the GPU clips on its own.
	//
	// Also shifts both item->x/y by the runtime menu-position offset
	// (Settings > Appearance > Position - Theme::kMenuOriginX/Y vs.
	// Theme::kDefaultMenuOriginX/Y, the base every content Grid's own
	// hardcoded origin assumes) - applied here, the one shared choke
	// point every content Grid's own draw()/drawText()/findItemAt()
	// (and MenuGrid's own, being a Grid itself) already funnels through,
	// rather than baked into GridRenderer's own PosH2C. PosH2C is also
	// what every non-menu overlay draws through (Notifications,
	// MenuPopup, MenuCommandBox, Onboarding), none of which should move
	// just because the menu did - this function is reached only from
	// Grid::draw()/drawText()/findItemAt(), so it's exactly menu-only.
	// The visibility cull below still compares against the unshifted
	// origin.y - screen position and "how much has scrolled past the
	// top edge" are independent concerns.
	template<typename Fn>
	static void forEachVisibleItem(std::vector<std::unique_ptr<GridItem>>& items, const Position2d& origin, int16_t scrollOffset, Fn&& fn)
	{
		const auto offsetX = static_cast<int16_t>(Theme::kMenuOriginX - Theme::kDefaultMenuOriginX);
		const auto offsetY = static_cast<int16_t>(Theme::kMenuOriginY - Theme::kDefaultMenuOriginY);

		for (auto& item : items)
		{
			const auto shiftedY = static_cast<int16_t>(item->y - scrollOffset);
			if (shiftedY < origin.y)
				continue;

			const auto originalX = item->x;
			const auto originalY = item->y;
			item->x = static_cast<int16_t>(originalX + offsetX);
			item->y = static_cast<int16_t>(shiftedY + offsetY);
			fn(*item);
			item->x = originalX;
			item->y = originalY;
		}
	}

	void Grid::draw()
	{
		checkWatchedConditions();
		ensurePopulated();

		forEachVisibleItem(*items, origin, m_ScrollOffset, [](GridItem& item) {
			item.draw();
		});
	}

	void Grid::drawText()
	{
		checkWatchedConditions();
		ensurePopulated();

		forEachVisibleItem(*items, origin, m_ScrollOffset, [](GridItem& item) {
			item.drawText();
		});
	}

	GridItem* Grid::findItemAt(int16_t cursor_x, int16_t cursor_y)
	{
		checkWatchedConditions();
		ensurePopulated();

		if (m_ScrollOffset == 0)
			return getOccupant(*items, cursor_x, cursor_y);

		GridItem* found = nullptr;
		forEachVisibleItem(*items, origin, m_ScrollOffset, [&](GridItem& item) {
			if (!found && item.occupies(cursor_x, cursor_y))
				found = &item;
		});
		return found;
	}

	std::vector<GridItem*> Grid::getFocusableItems()
	{
		ensurePopulated();

		std::vector<GridItem*> focusable;
		for (auto& item : *items)
		{
			if (item->isFocusable())
				focusable.push_back(item.get());
		}
		return focusable;
	}

	void Grid::getBounds(int16_t& x1, int16_t& y1, int16_t& x2, int16_t& y2) const
	{
		const_cast<Grid*>(this)->ensurePopulated();

		x1 = SHRT_MAX;
		y1 = SHRT_MAX;
		x2 = 0;
		y2 = 0;
		for (const auto& item : *items)
		{
			if (item->x < x1)
				x1 = item->x;
			if (item->y < y1)
				y1 = item->y;

			const auto item_x2 = static_cast<int16_t>(item->x + item->width);
			const auto item_y2 = static_cast<int16_t>(item->y + item->height);
			if (item_x2 > x2)
				x2 = item_x2;
			if (item_y2 > y2)
				y2 = item_y2;
		}
	}

	void Grid::getDimensions(int16_t& x, int16_t& y, int16_t& width, int16_t& height) const
	{
		getBounds(x, y, width, height);
		width -= x;
		height -= y;
	}

	void Grid::clampScroll(int16_t visibleHeight)
	{
		int16_t x1, y1, x2, y2;
		getBounds(x1, y1, x2, y2);

		const int totalHeight = y2 - y1;
		const int maxScroll = std::max(0, totalHeight - static_cast<int>(visibleHeight));
		m_ScrollOffset = static_cast<int16_t>(std::clamp(static_cast<int>(m_ScrollOffset), 0, maxScroll));
	}

	void Grid::ScrollBy(int16_t delta, int16_t visibleHeight)
	{
		ensurePopulated();

		m_ScrollOffset = static_cast<int16_t>(m_ScrollOffset + delta);
		clampScroll(visibleHeight);
	}

	void Grid::ScrollToShow(const GridItem* item, int16_t visibleHeight)
	{
		if (!item)
			return;

		ensurePopulated();

		const auto viewTop = static_cast<int16_t>(origin.y + m_ScrollOffset);
		const auto viewBottom = static_cast<int16_t>(viewTop + visibleHeight);

		if (item->y < viewTop)
			m_ScrollOffset = static_cast<int16_t>(m_ScrollOffset - (viewTop - item->y));
		else if (static_cast<int16_t>(item->y + item->height) > viewBottom)
			m_ScrollOffset = static_cast<int16_t>(m_ScrollOffset + ((item->y + item->height) - viewBottom));

		clampScroll(visibleHeight);
	}
}
