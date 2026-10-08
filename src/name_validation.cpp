#include "name_validation.hpp"

namespace Stand
{
	std::string filter_name(const std::string& name) { return name; }
	bool does_name_have_colour_prefix(const std::string& name) { return false; }
	std::string filter_name_pretty(const std::string& name) { return name; }
	bool is_name_length_valid(const std::string& name) { return true; }
	bool is_name_valid(const std::string& name) { return true; }
}
