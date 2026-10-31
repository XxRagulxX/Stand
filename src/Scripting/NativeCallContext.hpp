#pragma once

#include <cstdint>
#include "Scripting/scrNativeHandler.hpp"

namespace Stand
{
	class NativeCallContext : public rage::scrNativeCallContext
	{
	private:
		uint64_t m_return_stack[10] = { 0 };
		uint64_t m_arg_stack[100] = { 0 };

	public:
		NativeCallContext()
		{
			m_ReturnValue = &m_return_stack[0];
			m_Args = &m_arg_stack[0];
		}

		[[nodiscard]] static constexpr bool canInvoke(uint64_t hash) noexcept
		{
			return true;
		}
	};
}
