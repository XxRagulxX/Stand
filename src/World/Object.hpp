#pragma once
#include "Game/Entity.hpp"

namespace Stand
{
	class Object : public Entity
	{
	public:
		using Entity::Entity;

		static Object Create(uint32_t model, rage::fvector3 coords);
		bool IsCamera();
		bool IsSignalJammer();
	};
}