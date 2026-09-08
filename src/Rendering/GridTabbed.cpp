#include "Rendering/GridTabbed.hpp"

#include "Rendering/GridItemTabsHorizontal.hpp"

namespace Stand::Rendering
{
	void GridTabbed::InitTabs(std::vector<std::unique_ptr<GridItem>>& items_draft, int16_t height, std::vector<std::string> labels, std::vector<Grid*> targets, size_t activeIndex)
	{
		m_Targets = std::move(targets);

		auto tabs = std::make_unique<GridItemTabsHorizontal>(static_cast<float>(height), std::move(labels), activeIndex);
		m_Tabs = tabs.get();
		items_draft.push_back(std::move(tabs));
	}

	void GridTabbed::draw()
	{
		Grid::draw();

		if (m_Tabs && m_Tabs->GetActiveIndex() < m_Targets.size())
		{
			if (auto* target = m_Targets[m_Tabs->GetActiveIndex()])
				target->draw();
		}
	}

	void GridTabbed::drawText()
	{
		Grid::drawText();

		if (m_Tabs && m_Tabs->GetActiveIndex() < m_Targets.size())
		{
			if (auto* target = m_Targets[m_Tabs->GetActiveIndex()])
				target->drawText();
		}
	}

	GridItem* GridTabbed::findItemAt(int16_t cursorX, int16_t cursorY)
	{
		if (auto* item = Grid::findItemAt(cursorX, cursorY))
			return item;

		if (m_Tabs && m_Tabs->GetActiveIndex() < m_Targets.size())
		{
			if (auto* target = m_Targets[m_Tabs->GetActiveIndex()])
				return target->findItemAt(cursorX, cursorY);
		}

		return nullptr;
	}

	std::vector<GridItem*> GridTabbed::getFocusableItems()
	{
		auto items = Grid::getFocusableItems();

		if (m_Tabs && m_Tabs->GetActiveIndex() < m_Targets.size())
		{
			if (auto* target = m_Targets[m_Tabs->GetActiveIndex()])
			{
				auto targetItems = target->getFocusableItems();
				items.insert(items.end(), targetItems.begin(), targetItems.end());
			}
		}

		return items;
	}
}
