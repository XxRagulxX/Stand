#include "Menu/Items.hpp"

namespace Stand
{
	TabItem::TabItem(const std::string& name) :
	    m_Name(name)
	{
	}

	void TabItem::Draw()
	{
		// No tab strip of its own to draw any more - TabBarItem now owns
		// deciding which tab is active and only calls into this one's
		// items when it is (see TabBarItem.cpp's own class comment).
		for (auto& item : m_Items)
		{
			if (item->CanDraw())
				item->Draw();
		}
	}
}
