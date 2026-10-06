#pragma once
#include "Rendering/StandPort/GridItem.hpp"

#include <soup/Canvas.hpp>
#include <string>

namespace Stand::Rendering
{
	class GridItemQrcode : public GridItem
	{
	private:
		soup::Canvas c;

	public:
		explicit GridItemQrcode(int16_t size, uint8_t priority, const std::string& text);

		void draw() final;
	};
}
