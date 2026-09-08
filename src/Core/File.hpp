#pragma once
#include "Game/BaseObj.hpp"

namespace Stand
{
	class File final : public BaseObj
	{
	public:
		File(const std::filesystem::path& path);

		File Move(std::filesystem::path newPath);
	};
}