#pragma once
#include "Rendering/StandPort/GridItem.hpp"

#include <DirectXMath.h>
#include <string>

namespace Stand::Rendering
{
	// A plain text label, drawn via GridRenderer::DrawText (DirectXTK12
	// SpriteFont). No background of its own - draw() is just the base
	// class's default no-op; only drawText() does anything, since text
	// is drawn in a separate pass (SpriteBatch) from solid rectangles
	// (PrimitiveBatch) - see GridRenderer::DrawImpl.
	class GridItemText : public GridItem
	{
	public:
		explicit GridItemText(std::string&& text, int16_t width, int16_t extra_padding, uint8_t priority, Alignment alignment_relative_to_last = ALIGN_BOTTOM_LEFT, GridItem* force_alignment_to = nullptr);

		GridItemText(int16_t width, int16_t height, std::string text, DirectX::XMFLOAT4 colour) :
		    GridItem(GRIDITEM_INDIFFERENT, width, height),
		    m_Text(std::move(text)),
		    m_Colour(colour)
		{
		}

		void drawText() override;

	private:
		std::string m_Text;
		DirectX::XMFLOAT4 m_Colour;
	};
}
