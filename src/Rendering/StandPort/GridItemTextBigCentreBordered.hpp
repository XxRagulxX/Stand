#pragma once
#include "Rendering/StandPort/GridItemTextBigCentre.hpp"

namespace Stand::Rendering
{
	class GridItemTextBigCentreBordered : public GridItemTextBigCentre
	{
	public:
		explicit GridItemTextBigCentreBordered(std::string&& text, int16_t width, int16_t height, uint8_t priority, Alignment alignment_relative_to_last = ALIGN_BOTTOM_LEFT, GridItem* force_alignment_to = nullptr);

		void draw() final;
	};
}
