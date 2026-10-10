#pragma once
#include "Rendering/StandPort/GridItemHeader.hpp"

namespace Stand::Rendering
{
	class GridItemHeaderLoading : public GridItemHeader
	{
	public:
		explicit GridItemHeaderLoading(int16_t width, uint8_t priority, GridItem* force_alignment_to);

		void draw() final;
		void drawText() final;
	};
}
