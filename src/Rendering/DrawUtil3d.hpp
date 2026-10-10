#pragma once

#include <SimpleMath.h>
#include "Game/vector.hpp"

namespace Stand
{
	struct DrawUtil3d
	{
		static void draw_line(const v3&, const v3&, int, int, int, int) {}
		static void draw_line_native(const v3&, const v3&, const DirectX::SimpleMath::Color&) {}
	};
}
