#pragma once

#include "Game/typedecl.hpp"

namespace Stand
{
	struct PlayerType
	{
		enum _ : playertype_t
		{
			SELF,
			FRIEND,
			STRANGER,
			SIZE
		};
	};
}
