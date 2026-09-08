#pragma once
#include "Rendering/Alignment.hpp"
#include "Rendering/GridItemType.hpp"

#include <climits>
#include <cstdint>
#include <string>

namespace Stand::Rendering
{
	// Ported from stand-reference's src/Menu/GridItem.hpp - same fields,
	// same names, same alignment-relative positioning contract (Grid::
	// setPositions() assigns x/y; onPositioned() is the hook a subclass
	// overrides to react). Two differences from Stand's real class:
	//
	// - draw() is split into draw()/drawText(), not one draw(). Stand's
	//   own D3D11 renderer defers every draw call into a sorted list and
	//   flushes it in one pass at frame end, so a single draw() is free
	//   to intermix rect and text calls in any order. This project's
	//   DirectXTK12 renderer instead opens one PrimitiveBatch (rects)
	//   and one SpriteBatch (text) per frame as two separate scopes -
	//   see GridRenderer::DrawImpl - so every item's rect work has to
	//   happen in one pass and its text work in another. Not a stylistic
	//   choice; the two batch types are different pipeline states that
	//   can't be freely interleaved within one Begin/End scope.
	// - onClick() exists here; Stand's own GridItem has no such hook,
	//   since Stand routes interactivity through its Command tree
	//   instead (a GridItemList's rows each resolve back to a Command,
	//   which is what actually reacts to a click) - a whole system this
	//   project hasn't ported. Until it has, GridItem owning its own
	//   click handling is this project's own addition, kept because
	//   every widget built on this base still needs some way to react
	//   to being clicked.
	// - isFocusable()/activate()/onArrow()/isKeyboardFocused() are this
	//   project's own addition too, for the same reason: Stand's own
	//   keyboard navigation moves a cursor through its Command tree
	//   (CommandList::m_cursor), which doesn't exist here. MenuFocus.hpp
	//   is this project's flattened equivalent - one focused item at a
	//   time within whatever content Grid MenuNavigation::Current()
	//   currently is - and these hooks are how a widget opts into it
	//   (isFocusable()), reacts to Enter (activate()) and to Left/Right
	//   (onArrow(), for a value a widget can cycle/step directly instead
	//   of just leaving focus - see e.g. GridItemCommandListSelect::onArrow()).
	class GridItem
	{
	public:
		const GridItemType type;
		int16_t x = SHRT_MIN;
		int16_t y = SHRT_MIN;
		int16_t width;
		int16_t height;
		uint8_t priority;
		Alignment alignment_relative_to_last;
		GridItem* force_alignment_to;
		bool keep_pos = false;

		explicit GridItem(GridItemType type,
		    int16_t width,
		    int16_t height,
		    uint8_t priority = 0,
		    Alignment alignment_relative_to_last = ALIGN_BOTTOM_LEFT,
		    GridItem* force_alignment_to = nullptr) :
		    type(type),
		    width(width),
		    height(height),
		    priority(priority),
		    alignment_relative_to_last(alignment_relative_to_last),
		    force_alignment_to(force_alignment_to)
		{
		}

		virtual ~GridItem() = default;

		// Pixel-space hit test against this item's assigned bounds -
		// Stand's own occupies(), used both for click dispatch here and
		// for Grid's own occupancy-conflict resolution during layout.
		[[nodiscard]] bool occupies(int16_t px, int16_t py) const
		{
			return x <= px && px <= (x + width) && y <= py && py <= (y + height);
		}

		// Called by Grid::setPositions() right after it assigns x/y -
		// Stand's own hook for a subclass that needs to react to being
		// (re)positioned. Default no-op, same as Stand's.
		virtual void onPositioned()
		{
		}

		// Solid-colour/background draws (PrimitiveBatch pass). Default
		// no-op: most items look different enough (button fill, toggle
		// indicator, ...) that there's no one shared "background" worth
		// defaulting to, unlike Stand's own draw() (which always paints
		// g_renderer.getBgRectColour() unless overridden).
		virtual void draw()
		{
		}

		// Text draws (SpriteBatch pass) - see the class comment above
		// for why this is separate from draw(). Default no-op: most
		// chrome items have no text of their own.
		virtual void drawText()
		{
		}

		// This project's own addition - see the class comment above.
		// Default no-op: most items don't respond to clicks.
		virtual void onClick(int16_t, int16_t)
		{
		}

		// The real entry point GridRenderer::WndProcImpl calls on
		// WM_LBUTTONDOWN, carrying the modifier keys held and whether
		// this is a double-click (time+target threshold, tracked in
		// GridRenderer) - Ctrl/Shift-click and double-click gestures used
		// throughout the classic menu (World > Spawn Ped's Ctrl+Click to
		// set player model, Teleport > Saved's double-click-to-teleport/
		// Shift-click-to-delete) that a plain onClick() has no way to
		// express. Default just calls onClick(x, y), ignoring every
		// gesture - every existing widget that only overrides onClick()
		// keeps behaving exactly as before; a widget that actually cares
		// about a gesture overrides this instead (and can still call
		// onClick(x, y) itself for the plain-click case).
		virtual void onClickEx(int16_t x, int16_t y, bool /*ctrl*/, bool /*shift*/, bool /*doubleClick*/)
		{
			onClick(x, y);
		}

		// Whether this item takes part in keyboard navigation at all -
		// see the class comment above. Default false: section labels
		// and chrome (GridItemText, GridItemAddressbar, ...) aren't
		// interactive and shouldn't ever receive focus.
		[[nodiscard]] virtual bool isFocusable() const
		{
			return false;
		}

		// Enter/Space while this item is the keyboard-focused one -
		// same effect as onClick() for most widgets (they share the
		// same underlying action, just without a cursor position to
		// derive which sub-element was hit - see e.g.
		// GridItemToggle::activate()). Default no-op.
		virtual void activate()
		{
		}

		// Left(-1)/Right(+1) while this item is the keyboard-focused
		// one, for a widget with its own directly-adjustable value
		// (GridItemCommandListSelect/GridItemIntStepper) - lets Left/Right
		// step it in place instead of just leaving focus. Returns
		// whether it was handled; MenuGrid::HandleKey() falls back to
		// moving focus back to the sidebar on an unhandled Left, same
		// as Backspace backing out of a nested category. Default
		// no-op, returning false (unhandled).
		virtual bool onArrow(int)
		{
			return false;
		}

		// Whether this item is the one keyboard focus currently sits
		// on - see MenuFocus.hpp. Only meaningful for an item that's
		// actually part of MenuNavigation::Current()'s own item list
		// (true of every content-Grid widget above); defined out of
		// line (GridItem.cpp) so this header doesn't need to pull in
		// MenuFocus.hpp/MenuNavigation.hpp itself.
		[[nodiscard]] bool isKeyboardFocused() const;

		// Whether this item is actively capturing free-text input right
		// now (see GridItemTextInput) - this project's own addition, for
		// the same reason isFocusable()/activate()/onArrow() are: Stand
		// routes free-text entry through its own separate CommandboxInput
		// system, a whole parallel input mode this project doesn't have.
		// GridRenderer::WndProcImpl checks this on the keyboard-focused
		// item (see isKeyboardFocused() above) to decide whether a
		// keystroke is text being typed (onChar()/onEditKey() below)
		// rather than list navigation (MenuGrid::HandleKey()). Default
		// false: only GridItemTextInput (and anything built on it, like
		// GridItemCommandInput) ever returns true.
		[[nodiscard]] virtual bool isEditingText() const
		{
			return false;
		}

		// A printable character typed while isEditingText() is true -
		// WM_CHAR's own wParam, verbatim. Default no-op.
		virtual void onChar(wchar_t)
		{
		}

		// A control key pressed while isEditingText() is true (Backspace/
		// Enter/Escape - see GridItemTextInput::onEditKey() for what each
		// one does there) - a plain Win32 virtual-key code, same
		// convention as MenuGrid::HandleKey()'s own vkCode. Default no-op.
		virtual void onEditKey(unsigned int)
		{
		}

		// Redirects isKeyboardFocused() to check proxy instead of this -
		// this project's own hook for GridItemConditional, which wraps
		// another GridItem without that item ever actually being the one
		// Grid places in its own item list (see GridItemConditional's own
		// class comment for why). Without this, the wrapped item's own
		// internal isKeyboardFocused() checks (used by its own draw()/
		// drawText() to decide when to show its own accent highlight)
		// would never match, since only the wrapper ever appears in
		// Grid::getFocusableItems()/MenuFocus. Pass nullptr to clear it.
		void setFocusProxy(GridItem* proxy)
		{
			m_FocusProxy = proxy;
		}

		// Help text for whatever real command backs this row (empty if
		// none, or if it has none) - this project's own addition, for
		// DescriptionPanel (Rendering/DescriptionPanel.hpp), which shows
		// it for whichever item MenuFocus::GetFocusedItem() currently
		// returns. Real Stand's own equivalent is
		// CommandPhysical::populateCorner() (confirmed against
		// origin/stand-reference), which every CommandPhysical already
		// participates in automatically since it owns its own help_text
		// directly - there's no such single base class here spanning
		// both this project's own Stand::Command tree and the ported
		// Stand::CommandPhysical one, so every GridItem that wraps
		// either kind overrides this itself instead. Default empty:
		// chrome (GridItemText, GridItemAddressbar, ...) and anything
		// not yet wired up have nothing to show.
		[[nodiscard]] virtual std::string GetDescription() const
		{
			return {};
		}

	private:
		GridItem* m_FocusProxy = nullptr;
	};
}
