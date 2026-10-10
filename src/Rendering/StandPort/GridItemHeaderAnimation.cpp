#include "Rendering/StandPort/GridItemHeaderAnimation.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/HeaderBanner.hpp"

namespace Stand::Rendering
{
	GridItemHeaderAnimation::GridItemHeaderAnimation(int16_t width, uint8_t priority, GridItem* force_alignment_to)
		: GridItemHeader(width, int16_t(HeaderBanner::GetRenderHeight(float(width))), priority, force_alignment_to)
	{
	}

	void GridItemHeaderAnimation::draw()
	{
		GridItem::draw();
		GridRenderer::QueueHeaderDraw(float(x), float(y), float(width), float(height));
	}
}
