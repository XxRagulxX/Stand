#include "Rendering/MenuFocus.hpp"

#include "Rendering/Grid.hpp"
#include "Rendering/GridItem.hpp"

namespace Stand::Rendering
{
	MenuFocus::Region MenuFocus::s_Region = MenuFocus::Region::Sidebar;
	Grid* MenuFocus::s_LastContent = nullptr;
	size_t MenuFocus::s_ContentIndex = 0;

	MenuFocus::Region MenuFocus::GetRegion()
	{
		return s_Region;
	}

	void MenuFocus::SetRegion(Region region)
	{
		s_Region = region;
	}

	GridItem* MenuFocus::GetFocusedItem(Grid* currentContent)
	{
		if (!currentContent)
			return nullptr;

		// The current content Grid changed since we last looked (a
		// sidebar switch, or a GridItemFolder Push()/Backspace Pop()) -
		// see the class comment in MenuFocus.hpp for why this is
		// detected here instead of every call site that can change
		// MenuNavigation's current Grid remembering to reset this too.
		if (currentContent != s_LastContent)
		{
			s_LastContent = currentContent;
			s_ContentIndex = 0;
		}

		const auto focusable = currentContent->getFocusableItems();
		if (focusable.empty())
			return nullptr;

		if (s_ContentIndex >= focusable.size())
			s_ContentIndex = focusable.size() - 1;

		return focusable[s_ContentIndex];
	}

	void MenuFocus::MoveContent(Grid* currentContent, int delta)
	{
		if (!currentContent)
			return;

		// Runs the same "did the content Grid change" check as
		// GetFocusedItem() before we trust s_ContentIndex below - its
		// return value isn't needed here, just the side effect.
		GetFocusedItem(currentContent);

		const auto focusable = currentContent->getFocusableItems();
		if (focusable.empty())
			return;

		const auto count = static_cast<int>(focusable.size());
		auto index = (static_cast<int>(s_ContentIndex) + delta) % count;
		if (index < 0)
			index += count;
		s_ContentIndex = static_cast<size_t>(index);
	}

	void MenuFocus::SetFocusedItem(Grid* currentContent, const GridItem* item)
	{
		if (!currentContent || !item)
			return;

		const auto focusable = currentContent->getFocusableItems();
		for (size_t i = 0; i < focusable.size(); ++i)
		{
			if (focusable[i] == item)
			{
				s_LastContent = currentContent;
				s_ContentIndex = i;
				s_Region = Region::Content;
				return;
			}
		}
	}
}
