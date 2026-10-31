#include "Rendering/StandPort/GridItemTextBigCentre.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	GridItemTextBigCentre::GridItemTextBigCentre(std::string&& text, int16_t width, int16_t height, uint8_t priority, Alignment alignment_relative_to_last, GridItem* force_alignment_to)
		: GridItem(GRIDITEM_INDIFFERENT, width, height, priority, alignment_relative_to_last, force_alignment_to),
		  text(std::move(text))
	{
	}

	void GridItemTextBigCentre::draw()
	{
		GridItem::draw();
		drawText();
	}

	void GridItemTextBigCentre::drawText() const
	{
		const float scale = Theme::kCommandTextScale;
		const auto size   = GridRenderer::MeasureText(text.c_str(), scale);
		const float tx    = x + (width / 2.f) + 1.f - (size.x / 2.f);
		const float ty    = y + (height / 2.f) - 2.f;
		GridRenderer::DrawText(tx, ty, text.c_str(), Theme::kUnfocusedText, scale);
	}
}
