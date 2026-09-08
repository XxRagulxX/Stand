#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"

namespace Stand::Hooks
{
	BOOL Anticheat::GetThreadContext(HANDLE hThread, LPCONTEXT lpContext)
	{
		auto ret = Hooking::Get<Anticheat::GetThreadContext>()->Original<decltype(&Anticheat::GetThreadContext)>()(hThread, lpContext);
		lpContext->Dr0 = 0;
		lpContext->Dr1 = 0;
		lpContext->Dr2 = 0;
		lpContext->Dr3 = 0;
		return ret;
	}
}