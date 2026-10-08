#pragma once

#include <cmath>
#include <cstdint>
#include <string>

#include <soup/Vector3.hpp>

#include "Scripting/scrVector.hpp"
#include "Util/hashtype.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef DEG_TO_RAD
#define DEG_TO_RAD(deg) ((deg) * (float(M_PI) / 180.0f))
#endif

#define CMDFLAG_BITS 23

namespace Stand
{
	using time_t = int64_t;
	using unixtime_t = int64_t;

	constexpr time_t TIME_MAX = 0x7FFFFFFFFFFFFFFF;

	using lang_t = uint8_t;

	using cursor_t = long long;

#if CMDFLAG_BITS == 15
	using commandflags_t = uint16_t;
#elif CMDFLAG_BITS == 23
	using commandflags_t = uint32_t;
#endif

	using toast_t = uint8_t;

	using punishment_t = uint32_t;

	using netGameEventId_t = uint16_t;
	using flowevent_t = netGameEventId_t;
	using floweventreaction_t = uint32_t;

	using playertype_t = uint8_t;
	using playerflag_t = uint32_t;

	using ToggleCorrelation_t = uint8_t;
}

namespace rage
{
	struct Vector2
	{
		float x = 0.0f;
		float y = 0.0f;

		constexpr Vector2() noexcept = default;
		constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}

		[[nodiscard]] constexpr bool isInBounds() const noexcept
		{
			return x >= 0.0f && x <= 1.0f && y >= 0.0f && y <= 1.0f;
		}
	};

	// nettypes.h
	using NetworkObjectType = uint16_t;
	using ObjectId = uint16_t;
}

namespace Stand
{
	void v_collectAxis(std::string& res, float axis);
	void v_collectAxis(std::wstring& res, float axis);
	[[nodiscard]] float v_getZFromHeightmap(float x, float y);
	extern void v_world2screen(float x, float y, float z, float* sx, float* sy);

	struct v3
	{
		float x{};
		float y{};
		float z{};

		constexpr v3() noexcept = default;
		constexpr v3(float x, float y, float z = 0.0f) noexcept : x(x), y(y), z(z) {}
		v3(const rage::scrVector& o) noexcept : x(o.x), y(o.y), z(o.z) {}
		v3(const soup::Vector3& o) noexcept : x(o.x), y(o.y), z(o.z) {}

		constexpr v3 operator*(float s) const noexcept { return {x * s, y * s, z * s}; }
		constexpr v3 operator+(const v3& o) const noexcept { return {x + o.x, y + o.y, z + o.z}; }
		constexpr v3 operator-(const v3& o) const noexcept { return {x - o.x, y - o.y, z - o.z}; }
		constexpr v3& operator+=(const v3& o) noexcept { x += o.x; y += o.y; z += o.z; return *this; }
		constexpr v3& operator-=(const v3& o) noexcept { x -= o.x; y -= o.y; z -= o.z; return *this; }
		constexpr v3& operator*=(float s) noexcept { x *= s; y *= s; z *= s; return *this; }
		constexpr v3 operator-(float s) const noexcept { return {x - s, y - s, z - s}; }

		[[nodiscard]] float magnitude() const noexcept { return sqrtf(x * x + y * y + z * z); }
		[[nodiscard]] float distance(const v3& o) const noexcept { return (o - *this).magnitude(); }
		[[nodiscard]] float distanceTopdown(const v3& o) const noexcept { const float dx = o.x - x; const float dy = o.y - y; return sqrtf(dx * dx + dy * dy); }
		[[nodiscard]] bool isNull() const noexcept { return x == 0.0f && y == 0.0f && z == 0.0f; }

		[[nodiscard]] rage::Vector2 getScreenPos() const
		{
			rage::Vector2 sp;
			Stand::v_world2screen(x, y, z, &sp.x, &sp.y);
			return sp;
		}

		[[nodiscard]] v3 toDir() const noexcept
		{
			const float yaw_r = DEG_TO_RAD(z);
			const float pitch_r = DEG_TO_RAD(x) * -1.0f;
			return { cosf(pitch_r) * sinf(yaw_r) * -1.0f, cosf(pitch_r) * cosf(yaw_r), sinf(pitch_r) * -1.0f };
		}

		[[nodiscard]] v3 toDirNoZ() const noexcept
		{
			const float yaw_r = DEG_TO_RAD(z);
			const float pitch_r = DEG_TO_RAD(x) * -1.0f;
			return { cosf(pitch_r) * sinf(yaw_r) * -1.0f, cosf(pitch_r) * cosf(yaw_r), 0.0f };
		}
	};
}

namespace rage
{
	struct Vector3 : public Stand::v3
	{
		float w{};

		using Stand::v3::v3;

		constexpr Vector3(float x, float y, float z, float w)
			: Stand::v3(x, y, z), w(w)
		{
		}

		void operator=(const Stand::v3& b) noexcept
		{
			x = b.x;
			y = b.y;
			z = b.z;
		}
	};

	using Vec3V = Vector3;
}

namespace Stand
{
	[[nodiscard]] inline v3 operator-(const rage::scrVector& a, const v3& b) noexcept
	{
		return { a.x - b.x, a.y - b.y, a.z - b.z };
	}
}
