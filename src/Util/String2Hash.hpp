#pragma once

#include "lib/soup/base.hpp"

#include "Game/hashtype.hpp"

namespace Stand
{
	union String2Hash
	{
		const char* str;
		struct
		{
			hash_t hash;
			hash_t remain;
		};

		constexpr String2Hash(const char* str)
			: str(str)
		{
		}

		[[nodiscard]] constexpr bool isInHashForm() const noexcept
		{
			return remain == 0;
		}

		operator hash_t() const noexcept
		{
			return hash;
		}

		void convertToHash();
	};
	static_assert(sizeof(String2Hash) == sizeof(const char*));

	// fmt no longer treats an implicit conversion operator (operator
	// hash_t() above) as enough to make a type formattable on its own -
	// it needs a format_as() overload it can find via ADL instead.
	inline auto format_as(const String2Hash& v) noexcept -> hash_t
	{
		return static_cast<hash_t>(v);
	}
}
