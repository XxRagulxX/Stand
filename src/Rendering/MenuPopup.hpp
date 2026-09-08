#pragma once
#include <functional>
#include <string>

namespace Stand::Rendering
{
	// A single modal Yes/No confirmation dialog, drawn on top of
	// everything else in MenuGrid - this project's first (and, for now,
	// only) floating overlay, standing in for the various
	// ImGui::OpenPopup/BeginPopupModal confirm dialogs scattered through
	// the classic menu (Saved Vehicles' spawn confirm, Teleport Saved's
	// delete confirm, ...). Deliberately minimal - one popup at a time,
	// Yes/No only, no nesting, no dedicated GridItem/Grid of its own -
	// rather than a general popup/window system; extend this (or add a
	// sibling type) if something ever needs more than a plain confirm
	// dialog.
	//
	// A free-standing global (not a GridItem, not owned by any Grid),
	// same shape as MenuFocus/MenuNavigation: GridRenderer::DrawImpl
	// draws it last (after MenuGrid, in the same rect/text passes) while
	// open, and GridRenderer::WndProcImpl routes every key to it first
	// while open, swallowing all of them rather than letting them reach
	// whatever's underneath - a popup being open always takes over
	// input, matching ImGui's own modal-popup behaviour. Keyboard-only,
	// same as the rest of this menu - see HandleKey() below.
	class MenuPopup
	{
	public:
		// Opens a Yes/No confirmation with the given message. onYes runs
		// if the user picks "Yes"; onNo (may be null) runs if they pick
		// "No" or dismiss (Escape) instead. Replaces any popup already
		// open without running its callbacks.
		static void Confirm(std::string message, std::function<void()> onYes, std::function<void()> onNo = nullptr);

		static bool IsOpen();
		static void Close();

		static void Draw();
		static void DrawText();

		// Handles VK_RETURN (Yes) and VK_ESCAPE (No/dismiss) while open;
		// any other key is a no-op. Call this for every WM_KEYDOWN while
		// IsOpen() is true and don't forward the key anywhere else
		// regardless of which key it was - a popup being open always
		// swallows input rather than letting it reach whatever's
		// underneath.
		static void HandleKey(unsigned int vkCode);

	private:
		struct Layout
		{
			float x, y, width, height;
			float yesX, noX, buttonY, buttonWidth, buttonHeight;
		};
		static Layout ComputeLayout();

		static bool s_Open;
		static std::string s_Message;
		static std::function<void()> s_OnYes;
		static std::function<void()> s_OnNo;
	};
}
