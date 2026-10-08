#pragma once

#include "player_tags.hpp"

namespace Stand
{
	class CommandPlayer
	{
	public:
		inline static const wchar_t* flag_prefix = L" [";
		inline static const wchar_t* flag_suffix = L"]";
		static char32_t flag_chars[FLAG_COUNT];

		bool force_recreate = false;
	};
}
