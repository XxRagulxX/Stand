#pragma once

#include <string>

#include "Game/typedecl.hpp"

namespace Stand
{
#pragma pack(push, 1)
	struct Vector2Plus
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		bool z_is_guessed = false;

		Vector2Plus() = default;
		Vector2Plus(float x, float y);
		Vector2Plus(float x, float y, float z);
		Vector2Plus(const rage::Vector2& b);
		Vector2Plus(const v3& b);
		Vector2Plus(const rage::scrVector& b);

		void operator=(const rage::Vector2& b);
		void operator=(const v3& b);
		void operator=(const rage::scrVector& b);
		void reset();

		operator v3() const;

		void guessZ();
		void unguessZ(float z);

		[[nodiscard]] bool isNull() const noexcept { return x == 0.0f && y == 0.0f; }

		operator std::string() const;
	};
#pragma pack(pop)
}
