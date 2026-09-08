#include "Rendering/GridItem.hpp"

#include "Rendering/MenuFocus.hpp"
#include "Rendering/MenuNavigation.hpp"

namespace Stand::Rendering
{
	bool GridItem::isKeyboardFocused() const
	{
		// Only meaningful for an item that's actually part of
		// MenuNavigation::Current()'s own item list - see the doc
		// comment in GridItem.hpp. Kept out of line so GridItem.hpp
		// itself doesn't need to pull in MenuFocus.hpp/MenuNavigation.hpp
		// (and, transitively, Grid.hpp) for every widget header that
		// includes it.
		//
		// Checks m_FocusProxy instead of `this` when set - see
		// setFocusProxy()'s own doc comment in GridItem.hpp for why
		// (GridItemConditional).
		return MenuFocus::GetFocusedItem(MenuNavigation::Current()) == (m_FocusProxy ? m_FocusProxy : this);
	}
}
