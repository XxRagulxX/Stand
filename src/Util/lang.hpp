#pragma once

#include <cstdint>
#include <string>

#include <fmt/format.h>

#include "Util/atStringHash.hpp"
#include "Util/LangData.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	using lang_t = uint8_t;

	enum LangId : lang_t
	{
		LANG_EN = 0,
	};

	struct Lang
	{
		inline static lang_t active_id = LANG_EN;
		inline static const LangData* active_data = nullptr;

		[[nodiscard]] static std::string get(const char* key) noexcept { return key; }
		[[nodiscard]] static std::string get(const std::string& key) noexcept { return key; }
		[[nodiscard]] static std::string get(const hash_t) noexcept { return {}; }
		[[nodiscard]] static std::wstring getW(const hash_t) noexcept { return {}; }
		[[nodiscard]] static std::string getEn(const hash_t) noexcept { return {}; }
		[[nodiscard]] static const LangData* id_to_data(const lang_t) noexcept { return nullptr; }
	};
}

#define LANG_GET(key)        Stand::Lang::get(key)
#define LANG_GET_W(key)      Stand::StringUtils::utf8_to_utf16(Stand::Lang::get(key))
#define LANG_GET_EN(key)     Stand::Lang::get(key)
#define LANG_FMT(key, ...)   fmt::format(fmt::runtime(LANG_GET(key)), __VA_ARGS__)
#define LANG_FMT_W(key, ...) fmt::format(fmt::runtime(LANG_GET_W(key)), __VA_ARGS__)
