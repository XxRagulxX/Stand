#pragma once

#include <string>

namespace Stand
{
	[[nodiscard]] extern std::string filter_name(const std::string& name);
	[[nodiscard]] extern bool does_name_have_colour_prefix(const std::string& name);
	[[nodiscard]] extern std::string filter_name_pretty(const std::string& name);
	[[nodiscard]] extern bool is_name_length_valid(const std::string& name);
	[[nodiscard]] extern bool is_name_valid(const std::string& name);
}
using Stand::filter_name_pretty;
using Stand::filter_name;
using Stand::does_name_have_colour_prefix;
using Stand::is_name_length_valid;
using Stand::is_name_valid;
