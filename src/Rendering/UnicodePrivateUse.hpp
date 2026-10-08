#pragma once

#include <string>

namespace Stand
{
	struct UnicodePrivateUse
	{
		static constexpr const wchar_t* rs = L"";
		static constexpr const wchar_t* wanted_star = L"";
		static constexpr const wchar_t* lock = L"";
		static constexpr const wchar_t* rs_verified = L"";
		static constexpr const wchar_t* rs_created = L"";
		static constexpr const wchar_t* blank_box = L"";
		static constexpr const wchar_t* newline = L"";
		static constexpr const wchar_t* reset = L"";

		static constexpr const wchar_t* tab_self = L"";
		static constexpr const wchar_t* tab_vehicle = L"";
		static constexpr const wchar_t* tab_online = L"";
		static constexpr const wchar_t* tab_players = L"";
		static constexpr const wchar_t* tab_world = L"";
		static constexpr const wchar_t* tab_game = L"";
		static constexpr const wchar_t* tab_stand = L"";

		static constexpr const wchar_t* replace_tmp = L"";

		static constexpr const wchar_t* custom_00 = L"";
		static constexpr const wchar_t* custom_01 = L"";
		static constexpr const wchar_t* custom_02 = L"";
		static constexpr const wchar_t* custom_03 = L"";
		static constexpr const wchar_t* custom_04 = L"";
		static constexpr const wchar_t* custom_05 = L"";
		static constexpr const wchar_t* custom_06 = L"";
		static constexpr const wchar_t* custom_07 = L"";
		static constexpr const wchar_t* custom_08 = L"";
		static constexpr const wchar_t* custom_09 = L"";
		static constexpr const wchar_t* custom_0A = L"";
		static constexpr const wchar_t* custom_0B = L"";
		static constexpr const wchar_t* custom_0C = L"";
		static constexpr const wchar_t* custom_0D = L"";
		static constexpr const wchar_t* custom_0E = L"";
		static constexpr const wchar_t* custom_0F = L"";
		static constexpr const wchar_t* custom_10 = L"";
		static constexpr const wchar_t* custom_11 = L"";
		static constexpr const wchar_t* custom_12 = L"";
		static constexpr const wchar_t* custom_13 = L"";
		static constexpr const wchar_t* custom_14 = L"";
		static constexpr const wchar_t* custom_15 = L"";
		static constexpr const wchar_t* custom_16 = L"";
		static constexpr const wchar_t* custom_17 = L"";
		static constexpr const wchar_t* custom_18 = L"";
		static constexpr const wchar_t* custom_19 = L"";
		static constexpr const wchar_t* custom_1A = L"";
		static constexpr const wchar_t* custom_1B = L"";
		static constexpr const wchar_t* custom_1C = L"";
		static constexpr const wchar_t* custom_1D = L"";
		static constexpr const wchar_t* custom_1E = L"";
		static constexpr const wchar_t* custom_1F = L"";

		static void fromGta(std::wstring& text);
		[[nodiscard]] static std::wstring fromGta(std::wstring&& text);
		static void toGta(std::wstring& text);
		[[nodiscard]] static std::wstring toGta(std::wstring&& text);
		static void destroyGta(std::wstring& text);
	};
}
