#include "Rendering/StandPort/GridItemHeaderLoading.hpp"

#include <fmt/format.h>

#include "Rendering/GridRenderer.hpp"
#include "Rendering/HeaderBanner.hpp"
#include "Rendering/HeaderLoadingSprite.hpp"
#include "Rendering/Theme.hpp"

namespace Stand::Rendering
{
	GridItemHeaderLoading::GridItemHeaderLoading(int16_t width, uint8_t priority, GridItem* force_alignment_to)
		: GridItemHeader(width, int16_t(HeaderLoadingSprite::GetRenderHeight(float(width))), priority, force_alignment_to)
	{
	}

	void GridItemHeaderLoading::draw()
	{
		GridItem::draw();
		GridRenderer::QueueHeaderLoadingDraw(float(x), float(y), float(width), float(height));
	}

	void GridItemHeaderLoading::drawText()
	{
		if (HeaderBanner::kLoadingGoal != 0)
		{
			const auto progress = fmt::format("{}/{}", HeaderBanner::kLoadingProgress, HeaderBanner::kLoadingGoal);
			const float scale   = Theme::kSmallTextScale;
			const float textW   = GridRenderer::MeasureText(progress.c_str(), scale).x;
			GridRenderer::DrawText(float(x + width - 10) - textW, float(y) + 5.f, progress.c_str(), Theme::kUnfocusedText, scale);
		}
	}
}
