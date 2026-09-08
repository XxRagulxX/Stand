#include "Menu/Items.hpp"

namespace Stand
{
	CollapsingHeaderItem::CollapsingHeaderItem(const std::string& name) :
	    m_Name(name)
	{
	}

	// No-op - see TabBarItem.cpp's own comment on why (not reachable
	// from Lua at all, so no need to recurse into its own items either).
	void CollapsingHeaderItem::Draw()
	{
	}
}
