#pragma once

#include "Menu/GridItem.hpp"

namespace Stand
{
	class GridItemColourBox : public GridItem
	{
	protected:
		const DirectX::SimpleMath::Color colour;

	public:
		explicit GridItemColourBox(uint8_t priority, Alignment alignment_relative_to_last, DirectX::SimpleMath::Color&& colour);

		void draw() override;
	};
}
