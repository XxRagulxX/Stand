#pragma once
#include "Rendering/GridItem.hpp"

#include <string>
#include <vector>

namespace Stand::Rendering
{
	class GridItemTabsVertical : public GridItem
	{
	public:
		GridItemTabsVertical(int16_t width, float entryHeight, std::vector<std::string> entries, size_t activeIndex) :
		    GridItem(GRIDITEM_TABS, width, static_cast<int16_t>(entryHeight * static_cast<float>(entries.size()))),
		    m_EntryHeight(entryHeight),
		    m_Entries(std::move(entries)),
		    m_ActiveIndex(activeIndex)
		{
		}

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		size_t GetActiveIndex() const
		{
			return m_ActiveIndex;
		}

		void MoveActive(int delta);

	private:
		float m_EntryHeight;
		std::vector<std::string> m_Entries;
		size_t m_ActiveIndex;
	};
}
