#include "Scripting/LuaLibrary.hpp"
#include "Scripting/LuaManager.hpp"

namespace Stand
{
	LuaLibrary::LuaLibrary()
	{
		LuaManager::RegisterLibrary(this);
	}
}