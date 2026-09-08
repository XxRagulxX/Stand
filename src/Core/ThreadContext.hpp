#pragma once

#include <cstdint>

namespace Stand
{
	enum ThreadContext : uint8_t
	{
		TC_SCRIPT_NOYIELD,
		TC_SCRIPT_YIELDABLE,
		TC_RENDERER,
		TC_WORKER,
		TC_WNDPROC,
		TC_OTHER,

		TC_APPLYDEFAULTSTATE = TC_SCRIPT_NOYIELD,
	};

	[[nodiscard]] constexpr bool thread_context_is_script(ThreadContext thread_context)
	{
		return thread_context <= TC_SCRIPT_YIELDABLE;
	}

	[[nodiscard]] constexpr bool thread_context_has_game_tls(ThreadContext thread_context)
	{
		return thread_context <= TC_RENDERER;
	}
}

// The Stand-ported Command backend (Click, CommandPhysical, ...) was
// written against Stand's own Stand::ThreadContext and uses it (and its
// TC_* enumerators) unqualified. Rather than keep a second, duplicate
// copy of the same enum under namespace Stand, re-export the one real
// definition above so both namespaces share it.
namespace Stand
{
	using Stand::ThreadContext;
	using enum Stand::ThreadContext;
	using Stand::thread_context_has_game_tls;
	using Stand::thread_context_is_script;
}