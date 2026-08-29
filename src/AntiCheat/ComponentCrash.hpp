#pragma once

#include "AntiCheat/Component.hpp"

#include "Core/AbstractEntity.hpp"

namespace Stand
{
	struct ComponentCrash : public Component
	{
		void addPatterns(PatternBatch& batch) final;
		[[nodiscard]] std::vector<DetourHook*> getHooks() final;

		// Train Crash
		AbstractEntity car1{};
		AbstractEntity car2{};
		AbstractEntity car3{};
	};
	inline ComponentCrash g_comp_crash;
}
