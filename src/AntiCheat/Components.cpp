#include "AntiCheat/Components.hpp"

#include "AntiCheat/ComponentCrash.hpp"
#include "AntiCheat/ComponentCrashPatch.hpp"
#include "AntiCheat/ComponentDrawPatch.hpp"
#include "AntiCheat/ComponentGpu.hpp"
#include "AntiCheat/ComponentNetcode.hpp"
#include "AntiCheat/ComponentOverflowPatch.hpp"
#include "AntiCheat/ComponentSavedata.hpp"
#include "AntiCheat/ComponentSpoofPos.hpp"

namespace Stand
{
	static Component* all_components[] = {
		&g_comp_crash,
		&g_comp_crashpatch,
		&g_comp_drawpatch,
		&g_comp_gpu,
		&g_comp_netcode,
		&g_comp_overflowpatch,
		&g_comp_savedata,
		&g_comp_spoofpos,
	};

	void Components::addPatterns(PatternBatch& batch)
	{
		for (auto& comp : all_components)
		{
			comp->addPatterns(batch);
		}
	}

	void Components::collectHooks(std::vector<DetourHook*>& hooks)
	{
		for (auto& comp : all_components)
		{
			for (auto& hook : comp->getHooks())
			{
				hooks.emplace_back(hook);
			}
		}
	}
}
