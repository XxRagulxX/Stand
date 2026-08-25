#pragma once

#include "AntiCheat/Component.hpp"

namespace Stand
{
	struct ComponentOverflowPatch : public Component
	{
		void addPatterns(PatternBatch& batch) final;
		[[nodiscard]] std::vector<DetourHook*> getHooks() final;
	};
	inline ComponentOverflowPatch g_comp_overflowpatch;
}
