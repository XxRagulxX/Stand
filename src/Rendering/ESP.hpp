#pragma once

namespace Stand
{
	class ESP
	{
	public:
		// Line pass (skeletons) plus collecting every text item this
		// frame - see ESP.cpp's own comment on why text is collected
		// here but drawn later, and Overlay.hpp's identical Draw()/
		// DrawText() split for the general shape.
		static void Draw();

		// Text pass - draws whatever Draw() collected this frame. Must
		// run after Draw(), same contract as Overlay/Notifications.
		static void DrawText();
	};
}
