#pragma once

#include <utility> // std::forward

#include "base.hpp"
#include "type_traits.hpp"

NAMESPACE_SOUP
{
	template <typename T>
	class UniquePtr
	{
	public:
		T* data = nullptr;

		UniquePtr() noexcept = default;

		UniquePtr(T* ptr) noexcept
			: data(ptr)
		{
		}

		UniquePtr(UniquePtr&& b) noexcept
			: data(b.data)
		{
			b.data = nullptr;
		}

		template <typename T2, SOUP_RESTRICT(std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>)>
		UniquePtr(UniquePtr<T2>&& b) noexcept
			: data(static_cast<T*>(b.data))
		{
			b.data = nullptr;
		}

		UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator=(const UniquePtr&) = delete;

		~UniquePtr() noexcept
		{
			delete data;
		}

		void reset() noexcept
		{
			delete data;
			data = nullptr;
		}

		void reset(T* ptr) noexcept
		{
			if (data != ptr)
			{
				delete data;
				data = ptr;
			}
		}

		UniquePtr& operator=(UniquePtr&& b) noexcept
		{
			if (this != &b)
			{
				delete data;

				data = b.data;
				b.data = nullptr;
			}

			return *this;
		}

		template <typename T2, SOUP_RESTRICT(std::is_base_of_v<T, T2> || std::is_base_of_v<T2, T>)>
		UniquePtr& operator=(UniquePtr<T2>&& b) noexcept
		{
			if (data != b.data)
			{
				delete data;

				data = static_cast<T*>(b.data);
				b.data = nullptr;
			}
			return *this;
		}

		[[nodiscard]] explicit operator bool() const noexcept
		{
			return data != nullptr;
		}

		[[nodiscard]] operator T*() const noexcept
		{
			return get();
		}

		[[nodiscard]] T* get() const noexcept
		{
			return data;
		}

		[[nodiscard]] T& operator*() const noexcept
		{
			return *data;
		}

		[[nodiscard]] T* operator->() const noexcept
		{
			return data;
		}

		[[nodiscard]] T* release() noexcept
		{
			T* ptr = data;
			data = nullptr;
			return ptr;
		}
	};

	template <typename T, typename...Args, SOUP_RESTRICT(!std::is_array_v<T>)>
	[[nodiscard]] UniquePtr<T> make_unique(Args&&...args)
	{
		return UniquePtr<T>(new T(std::forward<Args>(args)...));
	}
}
