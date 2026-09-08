#pragma once

namespace Stand
{
	class AutoDriveHUD
	{
	public:
		// Rect/line/polygon pass - also where the current snapshot is
		// read, entity tracking updated, and this frame's layout
		// computed/stored for DrawText() to use afterward. Must run
		// before DrawText() each frame - same contract as
		// Notifications/Overlay/ESP/ChatDisplay/Onboarding.
		static void Draw();

		// Text pass (the header's speed/target/phase/owner/lane-change
		// labels) - reads whatever layout Draw() computed this frame.
		// Must run after Draw().
		static void DrawText();
	};
}
