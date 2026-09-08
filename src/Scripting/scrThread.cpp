#include "Scripting/scrThread.hpp"
#include "Core/tlsContext.hpp"
#include "Scripting/Scripts.hpp"
#include "Scripting/scrProgram.hpp"

namespace rage
{
	std::int64_t* scrThread::Context::GetStackPtr(void* stack) noexcept
	{
		if (!stack || m_StackPointer == 0)
			return nullptr;

		return reinterpret_cast<std::int64_t*>(stack)
		    + (m_StackPointer - 1);
	}

	std::uint8_t* scrThread::Context::GetCodePtr() const noexcept
	{
		auto* program = Stand::Scripts::FindScriptProgram(
		    static_cast<std::uint32_t>(m_ScriptHash));

		if (!program)
			return nullptr;

		return program->GetCodeAddress(m_ProgramCounter);
	}

	std::int64_t* scrThread::GetStackPtr() noexcept
	{
		return m_Context.GetStackPtr(m_Stack);
	}

	scrThread* scrThread::GetRunningThread() noexcept
	{
		auto* tls = tlsContext::Get();

		return tls ? tls->m_CurrentScriptThread : nullptr;
	}
}