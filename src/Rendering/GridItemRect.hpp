#pragma once
#include "Rendering/GridItem.hpp"

#include <DirectXMath.h>

namespace Stand::Rendering
{
	// A plain solid-colour rectangle. The simplest possible concrete
	// GridItem, used to prove out the Grid/GridItem tree before real
	// widget types (text, buttons, tabs, ...) land.
	class GridItemRect : public GridItem
	{
	public:
		GridItemRect(int16_t width, int16_t height, DirectX::XMFLOAT4 colour) :
		    GridItem(GRIDITEM_INDIFFERENT, width, height),
		    m_Colour(colour)
		{
		}

		void draw() override;

	private:
		DirectX::XMFLOAT4 m_Colour;
	};
}
