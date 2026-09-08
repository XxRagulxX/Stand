#include "Core/DetourHook.hpp"
#include "Scripting/ScriptMgr.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"
#include "Scripting/Scripts.hpp"

namespace Stand::Hooks
{
	bool Script::RunScriptThreads(int ops_to_execute)
	{
		auto res = Hooking::Get<Script::RunScriptThreads>()->Original<decltype(&RunScriptThreads)>()(ops_to_execute); // let's see what happens if we run our stuff after the game code instead of before
		if (g_Running)
		{
			auto thread = Scripts::FindScriptThread("freemode"_J);
			if (!thread)
				thread = Scripts::FindScriptThread("main_persistent"_J);
			if (!thread)
				thread = Scripts::FindScriptThread("startup"_J);
			if (thread)
			{
				Scripts::RunAsScript(thread, [] {
					//g_script_mgr.tick();;
					g_script_mgr.tick();
				});
			}
		}
		return res;
	}
}