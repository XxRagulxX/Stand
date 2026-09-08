#pragma once
#include <functional>
#include <string>

namespace Stand::Rendering
{
	// Stand-style floating "Command Box" for typing a value directly
	// instead of only stepping it with the row's own "-"/"+" buttons -
	// the user's own reference screenshot (real Stand, pressing Enter on
	// "Walk And Run Speed") shows a pink-titled "Stand Command Box"
	// overlay prefilled "walkspeed 1.00" with the option's label below
	// it. Real Stand's own command box (src/Commands/Widgets/
	// Commandbox.hpp / CommandboxGrid.cpp on origin/stand-reference) is a
	// full console, though: Textbox-based cursor placement + history
	// navigation, a colour-picker sub-mode, live fuzzy-matching
	// autocomplete against every registered command with its own
	// background caching thread, and ';'-chaining multiple commands in
	// one line. None of that is portable without Stand's own
	// CommandIssuable/CommandList tree this project doesn't have - the
	// same conclusion already reached porting CommandSlider itself (see
	// Commands/Widgets/'s own class comments). This is the scoped-
	// down version: one line of free text, prefilled "<command name>
	// <current value>" the same way real Stand's own
	// CommandSlider::openCommandBoxWithPrefill does, styled to match
	// (pink title bar, dark input row, muted label/range rows below) -
	// no cursor placement (append/backspace from the end only, same
	// trade-off as GridItemTextInput), no history, no autocomplete,
	// single command only.
	//
	// A free-standing global popup, same shape as MenuPopup (its own
	// class comment invites exactly this - "add a sibling type" - rather
	// than overloading Yes/No confirm for free text): GridRenderer::
	// DrawImpl draws it last while open, WndProcImpl routes every key to
	// it first while open, ahead of MenuPopup and the normal text-edit
	// interception - a command box open always takes over input, same
	// "modal" precedent MenuPopup already set.
	class MenuCommandBox
	{
	public:
		// Opens the box editing one command's value. commandName is the
		// real Command::GetName() (e.g. "walkspeed") - shown, and
		// editable, as the leading token of the one free-text line, same
		// as real Stand's own prefill; label is the display name shown
		// under the input (e.g. "Walk And Run Speed"); rangeText is an
		// already-formatted hint row (e.g. "Range: -10000 to 10000",
		// empty to omit the row - not every command has both bounds);
		// initialValue is just the value portion (e.g. "1.00"), combined
		// with commandName to build the full editable line.
		//
		// onSubmit runs once Enter is pressed, given whatever remains of
		// the line after its first whitespace-delimited token (the
		// command name, whatever the user left it as) is stripped and
		// trimmed - e.g. "1.00", or "min"/"max" if the caller wants to
		// support those keywords the way real Stand's own
		// CommandSlider::onCommandImplKeyword does (GridItemCommandSlider/
		// Float/CommandSliderNamedValue all do - see their own OpenCommandBox()
		// comments). Returns true if the value was valid and applied
		// (closes the box) or false if it wasn't (box stays open, an
		// inline "Invalid value." row flashes briefly) - the scoped-down
		// equivalent of real Stand's own INVARG response.
		static void Open(std::string commandName,
		    std::string label,
		    std::string rangeText,
		    std::string initialValue,
		    std::function<bool(const std::string&)> onSubmit);

		static bool IsOpen();
		static void Close();

		static void Draw();
		static void DrawText();

		// Enter/Escape/Backspace while open - same convention as
		// GridItem::onEditKey()'s own vkCode. Call for every WM_KEYDOWN
		// while IsOpen(), same as MenuPopup::HandleKey().
		static void HandleKey(unsigned int vkCode);

		// A typed character while open - same convention as GridItem::
		// onChar()'s own wParam. Call for every WM_CHAR while IsOpen().
		static void HandleChar(wchar_t c);

	private:
		struct Layout
		{
			float x, y, width;
			float titleY, titleHeight;
			float inputY, inputHeight;
			float labelY, labelHeight;
			float rangeY, rangeHeight;
			float errorY, errorHeight;
		};
		static Layout ComputeLayout();

		static bool s_Open;
		static std::string s_CommandName;
		static std::string s_Label;
		static std::string s_RangeText;
		static std::string s_Buffer;
		static std::function<bool(const std::string&)> s_OnSubmit;

		static bool s_ShowError;
		static unsigned long long s_ErrorShownAtMs;
	};
}
