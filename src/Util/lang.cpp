#include "Util/lang.hpp"

#include "Util/LangBuiltins.hpp"
#include "Util/LangData.hpp"
#include "Util/StringUtils.hpp"
#include "Util/atStringHash.hpp"

namespace Stand
{
	static LangDataMap lang_data_en(&LangBuiltins::en);

	lang_t Lang::active_id = LANG_EN;
	const LangData* Lang::active_data = &lang_data_en;

	std::string Lang::get(const char* key) noexcept
	{
		if (active_data)
			return active_data->get(key);
		return key;
	}

	std::string Lang::get(const std::string& key) noexcept
	{
		if (active_data)
			return active_data->get(key);
		return key;
	}

	std::string Lang::get(hash_t key) noexcept
	{
		if (active_data)
			return active_data->get(key);
		return {};
	}

	std::wstring Lang::getW(hash_t key) noexcept
	{
		if (active_data)
			return active_data->getW(key);
		return {};
	}

	std::string Lang::getEn(hash_t key) noexcept
	{
		return lang_data_en.get(key);
	}
}
