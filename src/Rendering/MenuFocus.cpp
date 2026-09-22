#include "Rendering/MenuFocus.hpp"

#include "Rendering/Grid.hpp"
#include "Rendering/GridItem.hpp"

namespace Stand::Rendering
{
	MenuFocus::Region MenuFocus::s_Region = MenuFocus::Region::Sidebar;
	Grid* MenuFocus::s_LastContent = nullptr;
	size_t MenuFocus::s_ContentIndex = 0;
	std::unordered_map<Grid*, size_t> MenuFocus::s_SavedIndices{};

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

		if (currentContent != s_LastContent)
		{
			if (s_LastContent)
				s_SavedIndices[s_LastContent] = s_ContentIndex;
			s_LastContent = currentContent;
			const auto it = s_SavedIndices.find(currentContent);
			s_ContentIndex = (it != s_SavedIndices.end()) ? it->second : 0;
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

	void MenuFocus::SaveFor(Grid* grid)
	{
		if (grid)
			s_SavedIndices[grid] = s_ContentIndex;
	}

	void MenuFocus::RestoreFor(Grid* grid)
	{
		if (!grid)
			return;
		const auto it = s_SavedIndices.find(grid);
		s_ContentIndex = (it != s_SavedIndices.end()) ? it->second : 0;
		s_LastContent = grid;
	}
}
