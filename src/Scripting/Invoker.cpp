#include "Scripting/Invoker.hpp"

#include "Scripting/Crossmap.hpp"
#include "Core/DetourHook.hpp"
#include "Core/Hooks.hpp"
#include "Core/Hooking.hpp"
#include "Scripting/scrProgram.hpp"

namespace Stand
{
	void NativeInvoker::DefaultHandler(rage::scrNativeCallContext* ctx)
	{
		LOG(FATAL) << "Native handler not registered";
		ctx->SetReturnValue(0);
	}

	void NativeInvoker::CacheHandlers()
	{
		if (m_AreHandlersCached)
			return;

		memcpy(m_Handlers.data(), g_Crossmap.data(), sizeof(g_Crossmap));
		auto program = reinterpret_cast<rage::scrProgram*>(calloc(1, sizeof(rage::scrProgram)));
		program->m_NativeCount = m_Handlers.size();
		program->m_NativeEntrypoints = m_Handlers.data();
		Hooking::Get<Hooks::Script::InitNativeTables>()->Original<decltype(&Hooks::Script::InitNativeTables)>()(program);
		free(program);
		m_AreHandlersCached = true;
	}
}