#pragma once

#include "Util/vector.hpp"

namespace Stand
{
#pragma pack(push, 1)
	struct Vector2Plus : public Vector3Base<Vector2Plus>
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
		Vector2Plus(const rage::scrVector3& b);

		void operator=(const rage::Vector2& b);
		void operator=(const v3& b);
		void operator=(const rage::scrVector3& b);
		void reset();

		operator v3() const;

		void guessZ();
		void unguessZ(float z);

		operator std::string() const;
	};
#pragma pack(pop)

	// A non-template overload beats VectorBase's format_as() template
	// (in Util/vector.hpp) in overload resolution, so this - not the
	// base class's own operator std::string() - is what fmt::format()
	// actually calls; Vector2Plus's own conversion accounts for
	// z_is_guessed and the others don't.
	inline auto format_as(const Vector2Plus& v) -> std::string
	{
		return static_cast<std::string>(v);
	}
}
