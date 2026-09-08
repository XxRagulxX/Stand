#pragma once
#include <cstdint>

namespace Stand::Rendering
{
	// Verbatim port of stand-reference's src/Menu/GridItemType.hpp -
	// lets Grid::getItemByType() find a specific item in a populated
	// list without every caller needing its own dynamic_cast. Most of
	// this system's widgets are GRIDITEM_INDIFFERENT (Stand's own
	// default for anything that isn't specifically one of the other
	// four) until something actually needs to look one up by type.
	enum GridItemType : uint8_t
	{
		GRIDITEM_INDIFFERENT,
		GRIDITEM_PRIMARYTEXT,
		GRIDITEM_LIST,
		GRIDITEM_SCROLLBAR,
		GRIDITEM_TABS,
	};
}
