#include "Rendering/StandPort/GridItemColourBoxBordered.hpp"

#include "Rendering/GridRenderer.hpp"

namespace Stand::Rendering
{
	static constexpr float kBorderWidth = 2.f;

	GridItemColourBoxBordered::GridItemColourBoxBordered(uint8_t priority, Alignment alignment_relative_to_last, DirectX::XMFLOAT4&& colour, DirectX::XMFLOAT4&& border_colour)
		: GridItemColourBox(priority, alignment_relative_to_last, std::move(colour)),
		  border_colour(std::move(border_colour))
	{
	}

	void GridItemColourBoxBordered::draw()
	{
		GridRenderer::DrawRect(x, y, width, height, border_colour);
		GridRenderer::DrawRect(
		    x + kBorderWidth,
		    y + kBorderWidth,
		    width  - kBorderWidth * 2.f,
		    height - kBorderWidth * 2.f,
		    colour);
	}
}
