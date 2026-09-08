#pragma once

#include "Game/vector.hpp"
#include <string>

namespace Stand::Features
{
	struct TeleportPlace
	{
		std::string name;
		rage::fvector3 position;
		float heading;
		bool requiresInterior = false;
		bool isGroundLevel = false;

		TeleportPlace(std::string n, rage::fvector3 pos, float h)
		    : name(std::move(n)), position(pos), heading(h)
		{
		}
	};

	namespace TeleportHelpers
	{
		void TeleportEntityTo(const TeleportPlace& place);

		TeleportPlace MakePlace(const char* name, float x, float y, float z, float heading);

		bool IsPlayerInInterior();
		bool IsValidCoordinate(const rage::fvector3& pos);
	}
}