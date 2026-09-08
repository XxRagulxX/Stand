#include "Menu/Submenu.hpp"

namespace Stand
{
	void Submenu::SetActiveCategory(const std::shared_ptr<Category> category)
	{
		m_ActiveCategory = category;
	}

	void Submenu::AddCategory(std::shared_ptr<Category>&& category)
	{
		if (!m_ActiveCategory)
			m_ActiveCategory = category;

		m_Categories.push_back(std::move(category));
	}

	void Submenu::RemoveCategory(const std::shared_ptr<Category>& category)
	{
		std::erase(m_Categories, category);
		if (m_ActiveCategory == category)
			m_ActiveCategory = m_Categories.empty() ? nullptr : m_Categories.front();
	}

	// No-op - neither of these is reachable from Lua at all any more
	// (unlike Group::Draw(), see its own comment) now that nothing
	// renders this tree - kept only so Category/Submenu still work as a
	// plain data model for Lua's own add_category()/find_category()/etc.
	// API (see Menu/UIManager.hpp's class comment).
	void Submenu::DrawCategorySelectors()
	{
	}

	void Submenu::Draw()
	{
	}
}
