#pragma once
#include "Game/BaseObj.hpp"

namespace Stand
{
	class File;
	class Folder final : public BaseObj
	{
	public:
		Folder(const std::filesystem::path& folder);

		File GetFile(std::filesystem::path file_path) const;
	};
}