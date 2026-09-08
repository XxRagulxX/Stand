#include "Menu/Items.hpp"

namespace Stand
{
	Group::Group(const std::string& name, int items_per_column) :
	    m_Name(name),
	    m_ItemsPerColumn(items_per_column)
	{
	}

	// Draws nothing of its own any more (see BoolCommandItem.cpp's own
	// comment on why) - still recurses into every child item's Draw(),
	// since this is the one reachable-from-Lua entry point into the
	// whole tree (Scripting/LuaMenu.cpp's group:draw()) and every child
	// item is itself a harmless no-op now, not because anything here
	// still needs walking.
	void Group::Draw()
	{
		for (auto& item : m_Items)
		{
			if (item->CanDraw())
				item->Draw();
		}
	}
}
