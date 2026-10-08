#pragma once

#include <string>

#include <fmt/core.h>
#include <fmt/xchar.h>

#include <soup/macros.hpp>
#include "Game/typedecl.hpp"

#include "Util/lang.hpp"

namespace Stand
{
	template <typename T>
	class BiString : public T
	{
	public:
		using T::T;

		BiString(T&& t)
			: T(std::move(t))
		{
		}

		static constexpr bool isA = std::is_same_v<T, std::string>;

		static BiString<T> fromLang(hash_t hash)
		{
			if constexpr (isA)
			{
				return BiString<T>(Lang::get(hash));
			}
			else
			{
				return BiString<T>(Lang::getW(hash));
			}
		}

		template <typename V>
		static BiString<T> fromValue(V val)
		{
			if constexpr (isA)
			{
				return BiString<T>(fmt::to_string(val));
			}
			else
			{
				return BiString<T>(fmt::to_wstring(val));
			}
		}
	};
}
