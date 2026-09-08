#include "Menu/Items.hpp"

namespace Stand
{
	TabBarItem::TabBarItem(const std::string& id) :
	    m_Id(id)
	{
	}

	// No-op - see BoolCommandItem.cpp's own comment on why. Unlike
	// Group::Draw(), nothing here is reachable from Lua at all (no
	// TabBarItem-returning binding exists in Scripting/LuaMenu.cpp), so
	// this doesn't even need to recurse into its own tabs' items.
	void TabBarItem::Draw()
	{
	}
}
