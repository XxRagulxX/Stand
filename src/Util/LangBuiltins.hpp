#pragma once

#include <unordered_map>

#include "Util/hashtype.hpp"

#include <string>

namespace Stand
{
	struct LangBuiltins
	{
		static std::unordered_map<hash_t, std::wstring> en;
	};
}
