#pragma once

#include <string>
#include <vector>

namespace Stand
{
	struct OverlayLine
	{
		std::string text;
		bool dimmed = false;
	};

	enum class OverlayPosition
	{
		TopLeft = 0,
		TopRight,
		BottomLeft,
		BottomRight,
		Free
	};

	class Overlay
	{
	public:
		static std::vector<OverlayLine> s_Lines;
		static OverlayPosition          s_Position;
		static float                    s_Tps;
		static float                    s_Dps;

		static void Draw();
		static void DrawText();
	};
}
