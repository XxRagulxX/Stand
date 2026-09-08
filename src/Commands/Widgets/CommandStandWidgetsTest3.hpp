#pragma once

#include <cstdint>

namespace Stand::Features
{
	// Read-back accessor for CommandStandWidgetsTest3.cpp's own shared
	// bitflag field - see that file's own comment. StandWidgetsTestGrid3
	// (Rendering/) watches this to prove each CommandToggleBitflag row only
	// ever touches its own bit.
	std::uint32_t GetTestFlags();
}
