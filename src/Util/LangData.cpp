#include "Util/LangData.hpp"

#include <soup/ObfusString.hpp>

#include "Util/atStringHash.hpp"
#include "Util/lang.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	LangData::LangData(const std::unordered_map<hash_t, std::wstring>* const map) noexcept
		: map(map)
	{
	}

	std::string LangData::get(const char* key) const noexcept
	{
		auto res = getImpl(rage::atStringHash(key));
		if (res.has_value())
		{
			return StringUtils::utf16_to_utf8(res.value());
		}
		return key;
	}

	std::string LangData::get(const std::string& key) const noexcept
	{
		auto res = getImpl(rage::atStringHash(key));
		if (res.has_value())
		{
			return StringUtils::utf16_to_utf8(res.value());
		}
		return key;
	}

	std::wstring LangData::getFallbackString() noexcept
	{
		return StringUtils::utf8_to_utf16(soup::ObfusString("/!\\ STRING NOT FOUND /!\\").str());
	}

	std::optional<std::wstring> LangDataMap::getImpl(const hash_t key) const noexcept
	{
		auto ent = map->find(key);
		if (ent != map->end())
		{
			return ent->second;
		}
		return {};
	}

	std::optional<std::wstring> LangEnUs::getImpl(const hash_t key) const noexcept
	{
		auto ent = map->find(key);
		if (ent != map->end())
		{
			std::wstring str = ent->second;

			StringUtils::replace_all(str, L"Tyre", L"Tire");

			StringUtils::replace_all(str, L"etre", L"eter");
			StringUtils::replace_all(str, L"ntre", L"nter");
			StringUtils::replace_all(str, L"ntring", L"ntering");

			StringUtils::replace_all(str, L"alise", L"alize");

			StringUtils::replace_all(str, L"isat", L"izat");

			StringUtils::replace_all(str, L"gorise", L"gorize");
			StringUtils::replace_all(str, L"mise", L"mize");
			StringUtils::replace_all(str, L"nise", L"nize");
			StringUtils::replace_all(str, L"itise", L"itize");

			StringUtils::replace_all(str, L"iour", L"ior");
			StringUtils::replace_all(str, L"mour", L"mor");
			StringUtils::replace_all(str, L"vour", L"vor");
			StringUtils::replace_all(str, L"alogue", L"alog");
			if (str.find(L"{") == std::wstring::npos)
			{
				StringUtils::replace_all(str, L"lour", L"lor");
			}

			StringUtils::replace_all(str, L"Grey", L"Gray");
			StringUtils::replace_all(str, L"Aluminium", L"Aluminum");
			StringUtils::replace_all(str, L"cancelled", L"canceled");

			return str;
		}
		return {};
	}

	std::optional<std::wstring> LangUwu::getImpl(const hash_t key) const noexcept
	{
		std::wstring result{};
		auto ent = map->find(key);
		if (ent != map->end())
		{
			result = StringUtils::owoifyWithFmtException(ent->second);
		}
		return result;
	}
}
