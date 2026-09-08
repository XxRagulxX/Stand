#include "Rendering/GridItemConditional.hpp"

#include "Commands/Commands.hpp"

namespace Stand::Rendering
{
	GridItemConditional::GridItemConditional(std::unique_ptr<GridItem> item, joaat_t boolCommandId, bool negate) :
	    GridItem(item->type, item->width, item->height, item->priority, item->alignment_relative_to_last, item->force_alignment_to),
	    m_Condition(Commands::GetCommand<CommandToggleLegacy>(boolCommandId)),
	    m_ConditionFn(nullptr),
	    m_Negate(negate),
	    m_Item(std::move(item))
	{
		// See setFocusProxy()'s own doc comment in GridItem.hpp - m_Item
		// itself never appears in a Grid's own item list (this wrapper
		// does, in its place), so its own isKeyboardFocused() checks
		// (used internally by its draw()/drawText()) need redirecting to
		// this instead of matching against itself.
		m_Item->setFocusProxy(this);
	}

	GridItemConditional::GridItemConditional(std::unique_ptr<GridItem> item, std::function<bool()> conditionFn, bool negate) :
	    GridItem(item->type, item->width, item->height, item->priority, item->alignment_relative_to_last, item->force_alignment_to),
	    m_Condition(nullptr),
	    m_ConditionFn(std::move(conditionFn)),
	    m_Negate(negate),
	    m_Item(std::move(item))
	{
		m_Item->setFocusProxy(this);
	}

	bool GridItemConditional::CanDraw() const
	{
		if (m_Condition)
			return m_Condition->GetState() != m_Negate;

		if (m_ConditionFn)
			return m_ConditionFn() != m_Negate;

		return false;
	}

	void GridItemConditional::onPositioned()
	{
		// Grid::setPositions() only ever sees this wrapper, never m_Item
		// directly (see the class comment in the header) - propagate the
		// position it just assigned us down to the wrapped item so its
		// own draw()/drawText()/onClick() (which all read x/y directly)
		// see the right place.
		m_Item->x = x;
		m_Item->y = y;
		m_Item->onPositioned();
	}

	void GridItemConditional::draw()
	{
		if (CanDraw())
			m_Item->draw();
	}

	void GridItemConditional::drawText()
	{
		if (CanDraw())
			m_Item->drawText();
	}

	void GridItemConditional::onClick(int16_t cursorX, int16_t cursorY)
	{
		if (CanDraw())
			m_Item->onClick(cursorX, cursorY);
	}

	void GridItemConditional::onClickEx(int16_t x, int16_t y, bool ctrl, bool shift, bool doubleClick)
	{
		if (CanDraw())
			m_Item->onClickEx(x, y, ctrl, shift, doubleClick);
	}

	bool GridItemConditional::isFocusable() const
	{
		// Hidden (CanDraw() false) means not focusable either - keeps
		// keyboard Up/Down from ever landing on (and highlighting) a row
		// nobody can see. See the class comment in the header for what
		// this doesn't fix (the row still reserves its own layout slot).
		return CanDraw() && m_Item->isFocusable();
	}

	void GridItemConditional::activate()
	{
		if (CanDraw())
			m_Item->activate();
	}

	bool GridItemConditional::onArrow(int delta)
	{
		return CanDraw() && m_Item->onArrow(delta);
	}

	bool GridItemConditional::isEditingText() const
	{
		return CanDraw() && m_Item->isEditingText();
	}

	void GridItemConditional::onChar(wchar_t c)
	{
		if (CanDraw())
			m_Item->onChar(c);
	}

	void GridItemConditional::onEditKey(unsigned int vkCode)
	{
		if (CanDraw())
			m_Item->onEditKey(vkCode);
	}
}
