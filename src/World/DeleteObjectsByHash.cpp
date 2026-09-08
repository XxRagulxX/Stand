#include "Scripting/Natives.hpp"
#include "Game/Pools.hpp"

namespace Stand
{
	void DeleteObjectsByHash(Hash hash)
	{
		for (auto object : Stand::Pools::GetObjects())
		{
			if (!object)
				continue;

			int handle = object.GetHandle();

			if (ENTITY::DOES_ENTITY_EXIST(handle) && ENTITY::GET_ENTITY_MODEL(handle) == hash)
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(handle, true, true);
				OBJECT::DELETE_OBJECT(&handle);
			}
		}
	}
}
