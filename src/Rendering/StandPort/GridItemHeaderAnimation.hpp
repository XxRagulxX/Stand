#pragma once
#include "Rendering/StandPort/GridItemHeader.hpp"

namespace Stand::Rendering
{
	class GridItemHeaderAnimation : public GridItemHeader
	{
	public:
		explicit GridItemHeaderAnimation(int16_t width, uint8_t priority, GridItem* force_alignment_to);

		void draw() final;
	};
}
