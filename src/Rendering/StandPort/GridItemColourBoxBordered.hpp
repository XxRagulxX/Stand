#pragma once
#include "Rendering/StandPort/GridItemColourBox.hpp"

namespace Stand::Rendering
{
	class GridItemColourBoxBordered : public GridItemColourBox
	{
	private:
		const DirectX::XMFLOAT4 border_colour;

	public:
		explicit GridItemColourBoxBordered(uint8_t priority, Alignment alignment_relative_to_last, DirectX::XMFLOAT4&& colour, DirectX::XMFLOAT4&& border_colour);

		void draw() final;
	};
}
