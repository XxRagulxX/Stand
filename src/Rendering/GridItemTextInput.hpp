#pragma once
#include "Rendering/GridItem.hpp"

#include <cstddef>
#include <functional>
#include <string>

namespace Stand::Rendering
{
	// A single-line, click-to-edit text field - this project's own
	// addition (Stand routes free-text entry through its own dedicated
	// CommandboxInput system, a whole separate input mode this project
	// doesn't have - see GridItem.hpp's own isEditingText()/onChar()/
	// onEditKey() doc comments and GridRenderer::WndProcImpl for the
	// equivalent built for this instead).
	//
	// Click or Enter starts editing; typed characters append to the end,
	// Backspace removes the last one - no cursor placement within the
	// string, appending/trimming from the end only, same "simplest
	// interaction that still works" trade-off as everywhere else in this
	// system without a real popup/gradient/etc (see e.g.
	// GridItemCommandListSelect's cycling "<"/">" in place of a real dropdown).
	// Enter commits (calls onCommit with the typed text) and stops
	// editing; Escape discards the typed buffer and stops editing
	// without calling onCommit, leaving the last-committed value shown.
	// Also auto-commits (not discards) if keyboard focus moves elsewhere
	// while still editing - a mouse click on another row, or Up/Down
	// navigating away - see draw()'s own isKeyboardFocused() check;
	// there's no separate "blur" event in this system to hook instead.
	//
	// onChange (optional) fires on every edit to the in-progress buffer -
	// each character typed, each Backspace - unlike onCommit, which only
	// fires once on Enter/auto-commit with the final value. This is what
	// a live-filtering search box needs (GridItemSelectList's search
	// field re-filters its rows on every keystroke, not just once the
	// user is done typing); a plain "type a value, press Enter" field
	// (every other GridItemTextInput use so far) just leaves this null.
	//
	// maxLength (optional, defaults to the original hardcoded 255 - see
	// onChar()'s own comment) - ported from real Stand's own
	// CommandInputTextLimitChars (Commands/Widgets/
	// CommandInputTextLimitChars.hpp on origin/stand-reference), rescoped
	// to this field's own inline typing instead of Stand's own Command
	// Box: that class only exists to reject an over-length value typed
	// into Stand's own console-style command box, which is how Stand
	// enters EVERY command's value, string or numeric alike - this
	// project has no equivalent "command box" pathway for CommandInput
	// specifically (only its numeric commands go through MenuCommandBox -
	// see GridItemCommandSlider.hpp's own class comment), so the same
	// "stop accepting more once you hit the limit" behaviour is enforced
	// directly against the field's own live m_Buffer instead. onLimitReached
	// (optional) fires once per rejected keystroke, standing in for
	// Stand's own LANG_FMT("CHARLIMIT", ...) response message - this
	// class doesn't know Notifications exists (kept generic, same reason
	// it doesn't know Command exists either), so a caller wanting that
	// warning shown wires it in here instead (see GridItemCommandInput.cpp).
	class GridItemTextInput : public GridItem
	{
	public:
		GridItemTextInput(int16_t width,
		    int16_t height,
		    std::string label,
		    std::string initialValue,
		    std::function<void(const std::string&)> onCommit,
		    std::function<void(const std::string&)> onChange = nullptr,
		    bool scrolling = false,
		    std::size_t maxLength = 255,
		    std::function<void()> onLimitReached = nullptr);

		void draw() override;
		void drawText() override;
		void onClick(int16_t cursorX, int16_t cursorY) override;

		bool isFocusable() const override
		{
			return true;
		}

		void activate() override;

		bool isEditingText() const override
		{
			return m_Editing;
		}

		void onChar(wchar_t c) override;
		void onEditKey(unsigned int vkCode) override;

		// The last committed value (Enter, or auto-commit on blur) -
		// what a caller needing "whatever's currently typed here" reads,
		// same as onCommit's own argument. Not live while mid-edit (see
		// onChange instead for that); a caller wanting the in-progress
		// buffer itself has no way to reach it, matching how nothing
		// else in this system reads a GridItemTextInput's state from
		// outside the onCommit/onChange callbacks either.
		const std::string& GetValue() const
		{
			return m_Value;
		}

		// Overwrites the committed value from outside (e.g. a "Populate
		// Name" button copying some other live value in) - does not run
		// onCommit and does not touch an in-progress edit's own buffer,
		// so typing into the field mid-edit still works exactly as
		// before; the new value only becomes visible once editing stops
		// (or immediately, if not currently editing).
		void SetValue(std::string value)
		{
			m_Value = std::move(value);
		}

	private:
		void BeginEditing();
		void Commit();
		void Cancel();

		std::string m_Label;
		std::string m_Value;  // committed value, shown when not editing
		std::string m_Buffer; // in-progress typed text, shown while editing
		std::function<void(const std::string&)> m_OnCommit;
		std::function<void(const std::string&)> m_OnChange;
		bool m_Editing = false;
		bool m_Scrolling = false;
		std::size_t m_MaxLength;
		std::function<void()> m_OnLimitReached;
	};
}
