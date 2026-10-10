#pragma once

#include <cstdint>

namespace rage
{
	using atHashValue = uint32_t;

	struct Vector2
	{
		float x = 0.0f, y = 0.0f;

		[[nodiscard]] bool isInBounds() const noexcept { return x >= 0.0f && x <= 1.0f && y >= 0.0f && y <= 1.0f; }
	};

	struct Vector3
	{
		float x, y, z;
	};
}
