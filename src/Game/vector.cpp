#include "Game/typedecl.hpp"

#include <fmt/xchar.h>

#include "Scripting/Natives.hpp"
#include "Util/StringUtils.hpp"
#include "Util/Util.hpp"

namespace Stand
{
	void v_collectAxis(std::string& res, float axis)
	{
		StringUtils::list_append(res, axis);
	}

	void v_collectAxis(std::wstring& res, float axis)
	{
		StringUtils::list_append(res, fmt::to_wstring(axis));
	}

	float v_getZFromHeightmap(float x, float y)
	{
		return PATH::GET_APPROX_HEIGHT_FOR_POINT(x, y);
	}

	void v_world2screen(float worldX, float worldY, float worldZ, float* screenX, float* screenY)
	{
		GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(worldX, worldY, worldZ, screenX, screenY);
	}
}
