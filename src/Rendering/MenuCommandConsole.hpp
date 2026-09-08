#pragma once
#include <functional>
#include <string>
#include <vector>

namespace Stand::Rendering
{
	// Stand-style blank "Command Console" - typing a prefix (e.g. "god")
	// live-lists every command whose own name starts with it (e.g.
	// "godmode - Immortality"), same live-narrowing feel as the user's own
	// reference screenshot of real Stand's own Commandbox (Commands/
	// Widgets/Commandbox.hpp + CommandboxGrid.cpp on origin/stand-
	// reference). NOT the same class as MenuCommandBox (that one always
	// opens already bound to one specific command, prefilled "<name>
	// <value>" - this one opens blank and is itself the thing that finds
	// a command by name).
	//
	// Matching logic ported from real Stand's own CommandList::
	// checkCommandNameMatch (CommandList.cpp) simplified for this
	// project's own flat registries (no tree, no OVERSHOT-driven
	// recursion into child lists - that only exists there for real
	// Stand's own nested command-name addressing, which nothing here
	// has): an exact name match short-circuits to a single result the
	// same way real Stand's own HIT does; otherwise every command with a
	// name starting with (and longer than) the typed text is collected,
	// real Stand's own GRAZED - sorted alphabetically by display name
	// (real Stand's own tree walk order isn't alphabetical, but this
	// project has no equivalent ordering to preserve). Each match
	// displays as "<name> - <label>", exactly CommandIssuable::
	// getCompletionHint()'s own format (confirmed against origin/
	// stand-reference).
	//
	// Searches BOTH of this project's own command registries - legacy
	// Stand::Commands::GetCommands() (one name per command) and
	// Stand::CommandRegistry::GetCommands() (every isPhysical() command,
	// which may carry several aliases - command_names - same as real
	// Stand). A previous pass here excluded Stand commands entirely,
	// reasoning that Stand::CommandIssuable::onCommand() (the chat-
	// command parsing entry point) is still an empty stub - true, but
	// irrelevant: CommandPhysical::onClick() is the actual generic
	// "activate as if clicked" entry point (already proven working by
	// CommandHotkeyDispatch and GridItemStandCommand, both of which
	// dispatch through it directly), and needs no chat-command parsing
	// at all for a plain no-argument activation. Excluding Stand
	// commands meant a migrated feature (e.g. Self > Immortality) silently
	// stopped being findable here the moment it moved off the legacy
	// registry - a real regression, not a disclosed gap, now fixed.
	//
	// A free-standing global overlay, drawn every frame regardless of
	// whether the main menu is open (GridRenderer::DrawImpl's own
	// "always drawn" group, alongside Notifications/Overlay/ESP/...) -
	// unlike MenuPopup/MenuCommandBox (which only ever open from within
	// an already-open menu), this one opens with its own dedicated 'U'
	// key and works whether or not the menu itself is open, matching
	// real Stand's own Gui::showCommandBox() (no g_gui.opened check
	// there either - confirmed against origin/stand-reference). Wired
	// through its own separate, ungated AddWindowProcedureCallback
	// registration in GridRenderer::Init() rather than the menu-gated
	// WndProcImpl every other overlay here goes through - see that
	// function's own comment for why.
	class MenuCommandConsole
	{
	public:
		// Opens blank - no prefill, unlike MenuCommandBox::Open().
		static void Open();

		static bool IsOpen();
		static void Close();

		static void Draw();
		static void DrawText();

		// Enter (runs the currently-selected match's own activate() -
		// see the Match struct below)/Escape (closes without acting)/
		// Up/Down (moves the selection)/Backspace, same convention as
		// MenuCommandBox::HandleKey()'s own vkCode. Call for every
		// WM_KEYDOWN while IsOpen().
		static void HandleKey(unsigned int vkCode);

		// A typed character while open - same convention as
		// MenuCommandBox::HandleChar()'s own wParam. Call for every
		// WM_CHAR while IsOpen().
		static void HandleChar(wchar_t c);

	private:
		struct Layout
		{
			float x, y, width;
			float titleY, titleHeight;
			float inputY, inputHeight;
			float firstRowY, rowHeight;
		};
		static Layout ComputeLayout();

		// One matched command, source-agnostic - built once per match in
		// UpdateMatches() rather than keeping a legacy Command*/Stand::
		// CommandPhysical* union around, since the two need genuinely
		// different activation code (Command::Call() vs
		// CommandPhysical::onClick(Click&), both queued onto a script
		// thread via FiberPool::queueJob() - see UpdateMatches()'s own
		// comment for why neither can run inline on this WndProc
		// callback thread).
		struct Match
		{
			std::string hint; // "<name> - <label>", ready to draw as-is
			std::function<void()> activate;
		};

		// Recomputes s_Matches/s_SelectedIndex from s_Buffer - called
		// after every edit (HandleChar/Backspace), not lazily from
		// Draw()/DrawText() - real Stand's own equivalent (
		// refreshCommandboxCacheOnce) runs on a background thread since
		// its own tree has thousands of commands; this project's own
		// registries are small enough (low hundreds) that a plain
		// synchronous scan on every keystroke is genuinely fine, not a
		// corner cut.
		static void UpdateMatches();

		static bool s_Open;
		static std::string s_Buffer;
		static std::vector<Match> s_Matches;
		static int s_SelectedIndex; // -1 if s_Matches is empty
	};
}
