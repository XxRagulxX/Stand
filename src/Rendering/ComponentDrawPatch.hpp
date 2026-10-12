#pragma once

namespace Stand
{
	struct ComponentDrawPatch
	{
		bool enabled = true;
		bool block_all = false;
	};
	inline ComponentDrawPatch g_comp_drawpatch{};
}
