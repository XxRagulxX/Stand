#pragma once

#include "Rendering/TextureBase.hpp"

namespace Stand
{
	struct TextureArray : public TextureBase
	{
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textures;
	};
}
