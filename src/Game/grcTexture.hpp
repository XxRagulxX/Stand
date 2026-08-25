#pragma once

#include "Util/struct_base.hpp"

#include "Game/pgBase.hpp"

namespace rage
{
	struct grcTexture : public pgBase
	{
		INIT_PAD(pgBase, 0x28) const char* name;
	};
	static_assert(sizeof(grcTexture) == 0x30);

	struct grcTexturePC : public grcTexture
	{
	};

	struct grcTextureDX11 : public grcTexturePC
	{
	};
}
