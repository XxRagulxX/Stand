#pragma once
#include "Rendering/StandPort/GridItem.hpp"

#include <DirectXMath.h>

namespace Stand::Rendering
{
	class GridItemColourBox : public GridItem
	{
	protected:
		const DirectX::XMFLOAT4 colour;

	public:
		explicit GridItemColourBox(uint8_t priority, Alignment alignment_relative_to_last, DirectX::XMFLOAT4&& colour);

		void draw() override;
	};
}
