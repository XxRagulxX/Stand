#pragma once
#include <cstdint>

namespace Stand::Rendering
{
	class DescriptionPanel
	{
	public:
		static void Draw(int16_t contentX, int16_t contentY, int16_t contentRightX, int16_t sidebarX, int16_t sidebarBottomY);
		static void DrawText(int16_t contentX, int16_t contentY, int16_t contentRightX, int16_t sidebarX, int16_t sidebarBottomY);
	};
}
