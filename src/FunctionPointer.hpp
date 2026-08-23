#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

template <typename T>
[[nodiscard]] inline void* function_pointer_to_void(T function)
{
	static_assert(std::is_pointer_v<T>);
	static_assert(std::is_function_v<std::remove_pointer_t<T>>);

	return reinterpret_cast<void*>(function);
}

template <typename T, typename M>
[[nodiscard]] inline std::size_t member_offset(M T::* member)
{
	return reinterpret_cast<std::uintptr_t>(
		&(reinterpret_cast<T*>(0)->*member)
		);
}