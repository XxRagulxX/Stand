#include "Core/DetourHook.hpp"
#include "Scripting/NativeHooks.hpp"
#include "Scripting/ScriptPatches.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"

#include "Scripting/scrProgram.hpp"

namespace Stand::Hooks
{
	void Script::InitNativeTables(rage::scrProgram* program)
	{
		Hooking::Get<Script::InitNativeTables>()->Original<decltype(&Script::InitNativeTables)>()(program);
		if (g_Running)
		{
			if (program->m_CodeBlocks && program->m_CodeSize)
			{
				NativeHooks::RegisterProgram(program);
				ScriptPatches::RegisterProgram(program);
			}
			else
			{
				// printing name causes crash
				// LOGF(VERBOSE, "InitNativeTables: skipping SHV script {}", program->m_Name);
			}
		}
	}
}