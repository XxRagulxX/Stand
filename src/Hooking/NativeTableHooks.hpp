#pragma once

#include "hashtype.hpp"
#include "natives_decl.hpp"

namespace Stand
{
	class NativeTableHooks
	{
	public:
		static void onTick();

		[[nodiscard]] static rage::scrNativeHandler og(rage::scrNativeHash native);

		static void createNativeEntrypointOverride(rage::scrNativeHash native, rage::scrNativeHandler entrypoint_override);
		static void createNativeEntrypointOverride(hash_t script, rage::scrNativeHash native, rage::scrNativeHandler entrypoint_override);

		static void removeNativeEntrypointOverride(rage::scrNativeHash native);
		static void removeNativeEntrypointOverride(rage::scrNativeHash native, rage::scrNativeHandler og_entrypoint);
		static void removeNativeEntrypointOverride(hash_t script, rage::scrNativeHash native);

		static void forceCleanup();
	};
}
