#pragma once
#include "Rendering/StandPort/GridItem.hpp"

#include <string>

namespace Stand::Rendering
{
	class GridItemCommandboxInput : public GridItem
	{
	private:
		const std::string text;

	public:
		explicit GridItemCommandboxInput(std::string&& text, int16_t width, uint8_t priority);

		void draw() final;
	};
}
