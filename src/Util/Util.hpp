#pragma once

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>

#include "Game/typedecl.hpp"

#define TOAST_ABOVE_MAP         ((Stand::toast_t)0b1)
#define TOAST_CONSOLE           ((Stand::toast_t)0b10)
#define TOAST_FILE              ((Stand::toast_t)0b100)
#define TOAST_WEB               ((Stand::toast_t)0b1000)
#define TOAST_CHAT              ((Stand::toast_t)0b10000)
#define TOAST_CHAT_TEAM         ((Stand::toast_t)0b100000)
#define TOAST_LOGGER            ((Stand::toast_t)0b1000000)
#define TOAST_DEFAULT           (TOAST_ABOVE_MAP | TOAST_WEB)
#define TOAST_ALL               (TOAST_DEFAULT | TOAST_LOGGER)

namespace Stand
{
	class Util
	{
	public:
		Util() = delete;

		[[nodiscard]] static float angles_dist(float a1, float a2) noexcept
		{
			return 180.0f - fabsf(fabsf(a1 - a2) - 180.0f);
		}

		[[nodiscard]] static bool is_bigmap_active() noexcept { return false; }

		[[nodiscard]] static std::string GET_LABEL_TEXT(const char* label, bool use_replacements = true) { return label; }
		[[nodiscard]] static std::string GET_LABEL_TEXT(const std::string& label, bool use_replacements = true) { return label; }

		[[nodiscard]] static std::string to_padded_hex_string(int32_t dec)
		{
			char buf[9];
			snprintf(buf, sizeof(buf), "%08X", static_cast<uint32_t>(dec));
			return buf;
		}

		[[nodiscard]] static std::string to_padded_hex_string(uint32_t dec)
		{
			char buf[9];
			snprintf(buf, sizeof(buf), "%08X", dec);
			return buf;
		}

		[[nodiscard]] static std::string to_padded_hex_string(int64_t dec)
		{
			char buf[17];
			snprintf(buf, sizeof(buf), "%016llX", static_cast<unsigned long long>(dec));
			return buf;
		}

		[[nodiscard]] static std::string to_padded_hex_string(uint64_t dec)
		{
			char buf[17];
			snprintf(buf, sizeof(buf), "%016llX", static_cast<unsigned long long>(dec));
			return buf;
		}

		[[nodiscard]] static std::string to_padded_hex_string_with_0x(int32_t dec)
		{
			char buf[11];
			snprintf(buf, sizeof(buf), "0x%08X", static_cast<uint32_t>(dec));
			return buf;
		}

		[[nodiscard]] static std::string to_padded_hex_string_with_0x(uint32_t dec)
		{
			char buf[11];
			snprintf(buf, sizeof(buf), "0x%08X", dec);
			return buf;
		}

		[[nodiscard]] static std::string to_padded_hex_string_with_0x(int64_t dec)
		{
			char buf[19];
			snprintf(buf, sizeof(buf), "0x%016llX", static_cast<unsigned long long>(dec));
			return buf;
		}

		[[nodiscard]] static std::string to_padded_hex_string_with_0x(uint64_t dec)
		{
			char buf[19];
			snprintf(buf, sizeof(buf), "0x%016llX", static_cast<unsigned long long>(dec));
			return buf;
		}

		static void toast(const std::string& message, toast_t flags = TOAST_DEFAULT) {}
		static void toast(std::string&& message, toast_t flags = TOAST_DEFAULT) {}
	};
}