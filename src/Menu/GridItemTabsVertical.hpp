#pragma once

#include "Menu/GridItem.hpp"

namespace Stand
{
	class GridItemTabsVertical : public GridItem
	{
	public:
		explicit GridItemTabsVertical(uint8_t priority, Alignment alignment_relative_to_last);

		void draw() final;
	};
}
