#include "Core/Pointers.hpp"
#include "Scripting/ScriptGlobal.hpp"

namespace Stand
{
	void* ScriptGlobal::Get() const
	{
		return Pointers.ScriptGlobals[m_Index >> 0x12 & 0x3F] + (m_Index & 0x3FFFF);
	}

	bool ScriptGlobal::CanAccess() const
	{
		return Pointers.ScriptGlobals && (m_Index >> 0x12 & 0x3F) < 0x40 && Pointers.ScriptGlobals[m_Index >> 0x12 & 0x3F];
	}
}