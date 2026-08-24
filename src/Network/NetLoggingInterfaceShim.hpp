#pragma once

#include "netLoggingInterface.hpp"

#include <cstdarg>

#include <fmt/core.h>

#include "Exceptional.hpp"
#include "FileLogger.hpp"

namespace Stand
{
	// Separate function so the fmt::format() std::string temporary isn't a destructible local in the __try'ing function below.
	static void logNetLoggingInterfaceMessage(const char* component, const char* label, const char* buffer)
	{
		g_logger.log(fmt::format("[{}] {}: {}", component, label, buffer));
	}

	class NetLoggingInterfaceShim : public rage::netLoggingInterface
	{
	private:
		const char* component;

	public:
		explicit NetLoggingInterfaceShim(const char* component)
			: component(component)
		{
		}

		void _0x30(const char* label, const char* data_fmt, ...) final
		{
			__try
			{
				va_list argp;
				va_start(argp, data_fmt);
				char buffer[256];
				vsprintf_s(buffer, data_fmt, argp);
				va_end(argp);
				logNetLoggingInterfaceMessage(component, label, (const char*)&buffer[0]);
			}
			__EXCEPTIONAL()
			{
			}
		}
	};
}
