#pragma once

#include <string>

namespace Stand
{
	struct FileLogger
	{
		void log(const std::string&) {}
		void log(std::string&&) {}
	};

	inline FileLogger g_logger{};
}
