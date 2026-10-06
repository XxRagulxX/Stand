#include "Rendering/StandPort/GridItemHeader.hpp"

namespace Stand::Rendering
{
	GridItemHeader::GridItemHeader(int16_t width, int16_t height, uint8_t priority, GridItem* force_alignment_to)
		: GridItem(GRIDITEM_INDIFFERENT, width, height, priority, ALIGN_TOP_CENTRE, force_alignment_to)
	{
	}
}
