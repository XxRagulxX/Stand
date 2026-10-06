#include "Rendering/StandPort/GridItemColourBox.hpp"

#include "Rendering/GridRenderer.hpp"

namespace Stand::Rendering
{
	static constexpr int16_t kColourSize  = 47;

	GridItemColourBox::GridItemColourBox(uint8_t priority, Alignment alignment_relative_to_last, DirectX::XMFLOAT4&& colour)
		: GridItem(GRIDITEM_INDIFFERENT, kColourSize, kColourSize, priority, alignment_relative_to_last),
		  colour(std::move(colour))
	{
	}

	void GridItemColourBox::draw()
	{
		GridRenderer::DrawRect(x, y, width, height, colour);
	}
}
