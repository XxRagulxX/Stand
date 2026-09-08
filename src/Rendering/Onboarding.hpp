#pragma once

namespace Stand
{
	// First-run welcome/session-mode popup: mirrors the pre-port ImGui
	// modal (BeginPopupModal, a radio choice plus two "open a link"
	// buttons and a Close button) via GridRenderer's own DirectXTK12
	// primitives and input instead. Same "always visible regardless of
	// any menu" shape as Notifications/Overlay/ESP/ChatDisplay, but the
	// only one of this batch that's genuinely interactive - Up/Down
	// moves between its four rows (Session Mode/Open GitHub/Open Matrix
	// Server/Close), Left/Right cycles the Session Mode row, Enter
	// activates whichever row is focused - the same Up/Down/Left/Right/
	// Enter model every other content Grid in this system already uses,
	// just not built on Grid/GridItem itself (this isn't a page in the
	// sidebar tree - it's a modal that has to show even before the menu
	// has ever been opened, exactly like the original ImGui popup did
	// regardless of GUI::IsOpen()).
	//
	// Because of that "even before the menu has ever been opened" need,
	// its own input handling is wired up independently of
	// GridRenderer::WndProcImpl (which only ever runs while GUI::IsOpen()
	// is true) - see GridRenderer::Init()'s own second
	// AddWindowProcedureCallback registration.
	class Onboarding
	{
	public:
		// Rect pass - also where the "just became visible" one-time
		// GUI::SetOnboarding(true) call happens (mirrors the original's
		// own static-lambda-runs-once idiom). Must run before DrawText()
		// each frame - same contract as Notifications/Overlay/ESP/
		// ChatDisplay.
		static void Draw();

		// Text pass - draws whatever this frame's (re-derived, not
		// cached - see Onboarding.cpp's own ComputeLayout()) layout
		// says. Must run after Draw().
		static void DrawText();

		// Handles Up/Down/Left/Right/Enter while onboarding is still
		// incomplete; a no-op once it's done. Call this for every
		// WM_KEYDOWN, unconditionally (see this class's own comment on
		// why its own registration bypasses GridRenderer::WndProcImpl's
		// usual GUI::IsOpen() gate).
		static void HandleKey(unsigned int vkCode);
	};
}
