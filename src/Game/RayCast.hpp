#pragma once
#include "Game/Entity.hpp"
#include "Core/types.hpp"

namespace Stand
{
	class RayCast
	{
	public:
		RayCast(Entity* entity, Vector3* coords);
		RayCast(Entity* entity);
		RayCast(Vector3* coords);
		bool Cast();

	private:
		Entity* m_Entity;
		Vector3* m_Coords;
	};
}