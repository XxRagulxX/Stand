#pragma once

namespace Stand::Rendering
{
	// Ported from real Stand's own CommandWithOnTickFocused<T> (Commands/
	// Widgets/CommandWithOnTickFocused.hpp on origin/stand-reference) - a
	// generic "run this every frame while the menu row is focused, stop
	// when it isn't" mixin. Real Stand's own version is a CRTP template
	// wrapping a Command subclass, since a Command node IS the row there
	// (Stand's own menu is a tree of Commands, walked directly by
	// GridItemList - see this project's own session history on that
	// architecture split). This project's Commands never know about
	// focus at all - only a GridItem does (GridItem::isKeyboardFocused()) -
	// so the equivalent hook belongs on the GridItem side instead, and as
	// a plain member rather than a base class: virtual-inheriting two
	// different GridItem-derived base classes (this mixin AND whatever
	// GridItemCommand* row a feature already subclasses, e.g.
	// GridItemCommandSlider) to reach both would need diamond-inheritance
	// machinery for no real benefit when the same three lines just work
	// as a member instead.
	//
	// Usage: call Update(isKeyboardFocused()) once per draw() call (draw()
	// runs every frame for every visible row already, so no extra timer
	// is needed), and act on its return value/JustBlurred() the same
	// frame - see GridItemCommandSliderProximity.cpp/
	// GridItemEntityPreview.cpp for both real consumers.
	class GridItemFocusTracker
	{
	public:
		// Returns true the one frame focus was just gained (the "onFocus"
		// moment) - every frame focus stays true afterward, a caller
		// wanting "run every tick while focused" just checks
		// isKeyboardFocused() itself directly rather than this return
		// value, same as real Stand's own onTickFocused() being called
		// unconditionally while its own `focused` flag is true.
		bool Update(bool currentlyFocused)
		{
			const bool justFocused = currentlyFocused && !m_WasFocused;
			m_JustBlurred = !currentlyFocused && m_WasFocused;
			m_WasFocused = currentlyFocused;
			return justFocused;
		}

		// True only on the one frame focus was just lost (the "onBlur"
		// moment, e.g. to run cleanup) - valid for the same frame
		// Update() was just called on, not after.
		bool JustBlurred() const
		{
			return m_JustBlurred;
		}

	private:
		bool m_WasFocused = false;
		bool m_JustBlurred = false;
	};
}
