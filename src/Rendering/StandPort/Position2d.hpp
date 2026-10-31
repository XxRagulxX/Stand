#pragma once
#include <climits>
#include <cstdint>

namespace Stand::Rendering
{
	// Verbatim port of stand-reference's src/Util/Position2d.hpp - a
	// Grid's own origin point. SHRT_MAX marks "unset"/invalid, matching
	// Stand's own sentinel exactly (GridItem uses SHRT_MIN for the same
	// purpose on its own x/y - also unchanged from Stand).
	struct Position2d
	{
		int16_t x = SHRT_MAX;
		int16_t y = SHRT_MAX;

		[[nodiscard]] bool isValid() const noexcept
		{
			return x != SHRT_MAX && y != SHRT_MAX;
		}

		void invalidate()
		{
			x = SHRT_MAX;
			y = SHRT_MAX;
		}

		[[nodiscard]] bool operator==(const Position2d& b) const
		{
			return x == b.x && y == b.y;
		}

		[[nodiscard]] bool operator!=(const Position2d& b) const
		{
			return !operator==(b);
		}
	};
}
