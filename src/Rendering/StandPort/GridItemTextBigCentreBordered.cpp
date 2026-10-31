#include "Rendering/StandPort/GridItemTextBigCentreBordered.hpp"

#include "Rendering/GridRenderer.hpp"

namespace Stand::Rendering
{
	static constexpr float kBorderWidth = 2.f;

	GridItemTextBigCentreBordered::GridItemTextBigCentreBordered(std::string&& text, int16_t width, int16_t height, uint8_t priority, Alignment alignment_relative_to_last, GridItem* force_alignment_to)
		: GridItemTextBigCentre(std::move(text), width, height, priority, alignment_relative_to_last, force_alignment_to)
	{
	}

	void GridItemTextBigCentreBordered::draw()
	{
		GridItem::draw();
		constexpr DirectX::XMFLOAT4 white{1.f, 1.f, 1.f, 1.f};
		GridRenderer::DrawRect(x,                        y,                         width,        kBorderWidth, white);
		GridRenderer::DrawRect(x,                        y + height - kBorderWidth, width,        kBorderWidth, white);
		GridRenderer::DrawRect(x,                        y,                         kBorderWidth, height,       white);
		GridRenderer::DrawRect(x + width - kBorderWidth, y,                         kBorderWidth, height,       white);
		drawText();
	}
}
