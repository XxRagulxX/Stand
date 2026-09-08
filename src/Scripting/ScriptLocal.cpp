#include "Scripting/ScriptLocal.hpp"

namespace Stand
{
	void* ScriptLocal::Get() const
	{
		return reinterpret_cast<uintptr_t*>((uintptr_t)m_StackPtr + (m_Index * sizeof(uintptr_t)));
	}

	bool ScriptLocal::CanAccess() const
	{
		return m_StackPtr != nullptr;
	}
}