#pragma once

#include "Game/datBase.hpp"
#include "Game/pgDictionary.hpp"
#include "Game/grcTexture.hpp"

namespace rage
{
	struct grmShaderGroup : public datBase
	{
		/* 0x08 */ pgDictionary<grcTexture>* texture_dict;
		PAD(0x10, 0x20) void* var_array; // rage::atArray<rage::grcEffectVarEntry>
	};
	static_assert(sizeof(grmShaderGroup) == 0x28);
}
