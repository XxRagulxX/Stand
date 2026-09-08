#include "Scripting/LuaResource.hpp"
#include "Scripting/LuaManager.hpp"

namespace Stand
{
	void LuaResourceType::Register()
	{
		m_Index = LuaManager::RegisterResourceType(this);
	}
}