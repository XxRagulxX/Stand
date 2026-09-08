#pragma once
#include "Game/vector.hpp"

#include <cmath>

namespace Stand::Math
{
	inline float DegToRad(float deg)
	{
		return static_cast<float>(3.14159265359 / 180.0 * deg);
	}

	inline rage::fvector3 RotationToDirection(rage::fvector3 rotation)
	{
		float x = DegToRad(rotation.x);
		float z = DegToRad(rotation.z);

		float num = std::abs(cos(x));

		return rage::fvector3{-sin(z) * num, cos(z) * num, sin(x)};
	}

	inline rage::fvector3 LookAt(const rage::fvector3& from, const rage::fvector3& to)
	{
		rage::fvector3 direction = to - from; // Use built-in operator-
		float distanceXY         = sqrt(direction.x * direction.x + direction.y * direction.y);

		float pitch = atan2(-direction.z, distanceXY) * (180.0f / 3.14159265f);
		float yaw   = atan2(direction.y, direction.x) * (180.0f / 3.14159265f);

		return rage::fvector3{pitch, 0.0f, yaw};
	}

	inline rage::fvector3 Lerp(const rage::fvector3& a, const rage::fvector3& b, float t)
	{
		return rage::fvector3{a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t};
	}

	// Keep only Length function to calculate vector magnitude
	inline float Length(const rage::fvector3& v)
	{
		return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline uint64_t GetTimeMillis()
	{
		using namespace std::chrono;
		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	}
}