#include "Rendering/StandPort/GridItemCommandboxInput.hpp"

#include "Rendering/GridRenderer.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	GridItemCommandboxInput::GridItemCommandboxInput(std::string&& text, int16_t width, uint8_t priority)
		: GridItem(GRIDITEM_INDIFFERENT, width, 0, priority),
		  text(GridRenderer::WrapText(std::move(text), Theme::kCommandBoxInputScale, float(width - 10)))
	{
		height = int16_t(GridRenderer::MeasureText(this->text.c_str(), Theme::kCommandBoxInputScale).y + 5.f);
	}

	void GridItemCommandboxInput::draw()
	{
		GridItem::draw();
		GridRenderer::DrawText(float(x) + 5.f, float(y), text.c_str(), Theme::kUnfocusedText, Theme::kCommandBoxInputScale);
	}
}
