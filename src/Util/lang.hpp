#pragma once

#include <cstdint>
#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-literal-operator"
#include <fmt/format.h>
#pragma clang diagnostic pop

#include "Util/LangData.hpp"

namespace Stand
{
	using lang_t = uint8_t;

	enum LangId : lang_t
	{
		LANG_EN = 0,
	};

	struct Lang
	{
		static lang_t active_id;
		static const LangData* active_data;

		[[nodiscard]] static std::string get(const char* key) noexcept;
		[[nodiscard]] static std::string get(const std::string& key) noexcept;
		[[nodiscard]] static std::string get(hash_t key) noexcept;
		[[nodiscard]] static std::wstring getW(hash_t key) noexcept;
		[[nodiscard]] static std::string getEn(hash_t key) noexcept;
		[[nodiscard]] static const LangData* id_to_data(lang_t) noexcept { return nullptr; }
	};
}

#define LANG_GET(key)        Stand::Lang::get(key)
#define LANG_GET_W(key)      Stand::StringUtils::utf8_to_utf16(Stand::Lang::get(key))
#define LANG_GET_EN(key)     Stand::Lang::get(key)
#define LANG_FMT(key, ...)   fmt::format(fmt::runtime(LANG_GET(key)), __VA_ARGS__)
#define LANG_FMT_W(key, ...) fmt::format(fmt::runtime(LANG_GET_W(key)), __VA_ARGS__)
