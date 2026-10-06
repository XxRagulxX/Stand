#pragma once
#include "Rendering/StandPort/GridItem.hpp"

#include <string>

namespace Stand::Rendering
{
	class GridItemTextBigCentre : public GridItem
	{
	private:
		const std::string text;

	public:
		explicit GridItemTextBigCentre(std::string&& text, int16_t width, int16_t height, uint8_t priority, Alignment alignment_relative_to_last = ALIGN_BOTTOM_LEFT, GridItem* force_alignment_to = nullptr);

		void draw() override;
	protected:
		void drawText() const;
	};
}
