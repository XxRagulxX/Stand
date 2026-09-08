#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Scripting/ScriptPatches.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	int Script::ScriptVM(uint64_t* stack, uint64_t** scr_globals, rage::scrProgram* program, void* ctx)
	{
		if (g_Running)
		{
			ScriptPatches::OnScriptVMEnter(program);
			auto res = Hooking::Get<Script::ScriptVM>()->Original<decltype(&Script::ScriptVM)>()(stack, scr_globals, program, ctx);
			ScriptPatches::OnScriptVMLeave(program);
			return res;
		}
		return Hooking::Get<Script::ScriptVM>()->Original<decltype(&Script::ScriptVM)>()(stack, scr_globals, program, ctx);
	}
}