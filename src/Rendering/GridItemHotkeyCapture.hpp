#pragma once
#include "Rendering/GridItem.hpp"

#include <chrono>
#include <string>

namespace Stand
{
	struct CommandLink;
}

namespace Stand::Rendering
{
	// One command's hotkey row for Settings > Hotkeys - mirrors
	// DrawHotkey.cpp's own per-row rendering (label + current chain +
	// capture), minus the "click a chip to remove just that one key"
	// gesture (this menu has no mouse - see HotkeysGrid.hpp's own class
	// comment for the "Clear" button that replaces it).
	//
	// The original captures by holding the mouse down on the row's own
	// button (ImGui::IsItemActive()), polling live key state every
	// frame it's held (DrawHotkey.cpp's own g_HotkeySystem.CreateHotkey()
	// call) and stopping the instant the mouse is released - there's no
	// mouse here, so Enter (activate()) instead *toggles* capture on/off,
	// and draw() (which, like the original's own render callback, runs
	// every frame this row is visible) polls while armed instead of a
	// held-mouse check. Two safety nets a toggle (rather than a held
	// gesture) needs that "release the mouse" made automatic in the
	// original:
	// - Losing keyboard focus while still armed auto-stops capture (same
	//   "no separate blur event, so check it in draw()" reasoning as
	//   GridItemTextInput's own auto-commit).
	// - A bounded timeout (kCaptureTimeout) auto-stops it regardless -
	//   covers closing the whole menu (Insert) while still armed, which
	//   stops draw() from running at all until the menu reopens, so
	//   nothing would otherwise ever clear HotkeySystem's own
	//   m_BeingModified flag (which a still-armed row leaves set,
	//   silently suppressing every hotkey menu-wide) until it does.
	class GridItemHotkeyCapture : public GridItem
	{
	public:
		GridItemHotkeyCapture(int16_t width, int16_t height, std::string label, CommandLink* link);

		void draw() override;
		void drawText() override;

		bool isFocusable() const override
		{
			return true;
		}

		// Enter: not capturing -> start (disarming whichever other row
		// was capturing, same "only one at a time" the original's own
		// held-mouse gesture already implies); capturing -> stop.
		void activate() override;

	private:
		void StartCapturing();
		void StopCapturing();

		std::string m_Label;
		CommandLink* m_Link;
		bool m_Capturing = false;
		std::chrono::steady_clock::time_point m_CaptureStart;

		// The one row currently capturing, if any - starting a new one
		// stops whichever this used to point at. Not owning.
		static inline GridItemHotkeyCapture* s_ActiveCapture = nullptr;
	};
}
