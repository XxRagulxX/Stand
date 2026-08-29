#pragma once

// fmt (somewhere around 10.x, well before the 12.2.0 this project now
// vendors) removed automatic formatting of a plain enum as its
// underlying integer - what used to be an implicit conversion is now a
// deliberate compile error (a deleted formatter<T> constructor, or an
// "unformattable" static assertion) unless the type either has its own
// fmt::formatter<T> specialization, or is reachable via fmt::underlying()
// at the call site. This codebase was written against 9.1.0 and has
// fmt::format()/LANG_FMT() call sites all over it - Command*, Game/e*.hpp
// natives enums, etc. - that pass an enum value directly and rely on the
// old implicit behavior; hunting down and fmt::underlying()-wrapping
// every one of them individually isn't practical.
//
// Restoring that old behavior as one blanket rule here - instead of at
// each call site - keeps every existing fmt::format()/fmt::to_string()/
// LANG_FMT() call across the codebase working exactly as it did before,
// for any enum, scoped or unscoped, without needing to touch the call
// site at all. Included from Core/common.hpp (the PCH), so it's visible
// everywhere without every file needing its own #include for this.

#include <fmt/base.h>

#include <type_traits>

template <typename T, typename Char>
struct fmt::formatter<T, Char, std::enable_if_t<std::is_enum<T>::value>>
	: fmt::formatter<std::underlying_type_t<T>, Char>
{
	template <typename FormatContext>
	auto format(T value, FormatContext& ctx) const
	{
		using Base = fmt::formatter<std::underlying_type_t<T>, Char>;
		return Base::format(static_cast<std::underlying_type_t<T>>(value), ctx);
	}
};
