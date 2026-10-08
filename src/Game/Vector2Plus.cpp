#include "Game/Vector2Plus.hpp"

#include <fmt/core.h>

#include "Scripting/Natives.hpp"

namespace Stand
{
	Vector2Plus::Vector2Plus(float x, float y)
		: x(x), y(y), z(PATH::GET_APPROX_HEIGHT_FOR_POINT(x, y)), z_is_guessed(true)
	{
	}

	Vector2Plus::Vector2Plus(float x, float y, float z)
		: x(x), y(y), z(z), z_is_guessed(false)
	{
	}

	Vector2Plus::Vector2Plus(const rage::Vector2& b)
		: Vector2Plus(b.x, b.y)
	{
	}

	Vector2Plus::Vector2Plus(const v3& b)
		: Vector2Plus(b.x, b.y, b.z)
	{
	}

	Vector2Plus::Vector2Plus(const rage::scrVector& b)
		: Vector2Plus(b.x, b.y, b.z)
	{
	}

	void Vector2Plus::operator=(const rage::Vector2& b)
	{
		this->x = b.x;
		this->y = b.y;
		this->z = v_getZFromHeightmap(b.x, b.y);
		z_is_guessed = true;
	}

	void Vector2Plus::operator=(const v3& b)
	{
		this->x = b.x;
		this->y = b.y;
		unguessZ(b.z);
	}

	void Vector2Plus::operator=(const rage::scrVector& b)
	{
		this->x = b.x;
		this->y = b.y;
		unguessZ(b.z);
	}

	void Vector2Plus::reset()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		z_is_guessed = true;
	}

	Vector2Plus::operator v3() const
	{
		return v3(x, y, z);
	}

	void Vector2Plus::guessZ()
	{
		z = PATH::GET_APPROX_HEIGHT_FOR_POINT(x, y);
		z_is_guessed = true;
	}

	void Vector2Plus::unguessZ(float z)
	{
		this->z = z;
		z_is_guessed = false;
	}

	Vector2Plus::operator std::string() const
	{
		if (!z_is_guessed)
		{
			return fmt::format("{}, {}, {}", x, y, z);
		}
		return fmt::format("{}, {}", x, y);
	}
}
