#include "Commands/Stand/CommandSetFont.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include <Windows.h>

#include "Commands/Widgets/CommandListActionItem.hpp"
#include "Core/ExceptionHandler.hpp"
#include "Core/FileMgr.hpp"
#include "Rendering/bin.hpp"
#include "Rendering/Theme.hpp"
#include "Rendering/StandPort/StandRendererCompat.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	static std::vector<CommandListActionItemData> font_options = {};

	static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEXW* lpelfe, NEWTEXTMETRICEXW* lpntme, DWORD FontType, LPARAM lParam)
	{
		std::wstring name = lpelfe->elfLogFont.lfFaceName;
		if (!name.empty())
		{
			if (name.at(0) == L'@')
			{
				name.erase(0, 1);
			}
			std::string menu_name = StringUtils::utf16_to_utf8(name);
			for (const auto& option : font_options)
			{
				if (option.menu_name.isLiteralString(menu_name))
				{
					return 1;
				}
			}
			StringUtils::simplify(name);
#if COMPACT_COMMAND_NAMES
			CommandName command_name = StringUtils::utf16_to_utf8(name);
#else
			CommandName command_name = std::move(name);
#endif
			font_options.emplace_back(CommandListActionItemData{ (long long)font_options.size(), LIT(std::move(menu_name)), { std::move(command_name) } });
		}
		return 1;
	}

	static std::vector<CommandListActionItemData> getFontOptions()
	{
		font_options.clear();
		LOGFONTW lf{};
		lf.lfFaceName[0] = L'\0';
		lf.lfCharSet = DEFAULT_CHARSET;
		HWND hwnd = GetForegroundWindow();
		HDC hDC = GetDC(hwnd);
		EnumFontFamiliesExW(hDC, &lf, (FONTENUMPROCW)&EnumFontFamExProc, 0, 0);
		ReleaseDC(hwnd, hDC);
		std::sort(font_options.begin(), font_options.end(), [](const CommandListActionItemData& a, const CommandListActionItemData& b)
		{
			return a.menu_name.literal_str < b.menu_name.literal_str;
		});
		return std::move(font_options);
	}

	static void bin_file_write(const std::filesystem::path& path, const uint8_t* data, std::size_t size)
	{
		std::ofstream f(path, std::ios::binary);
		f.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size));
	}

	CommandSetFont::CommandSetFont(CommandList* parent)
		: CommandListAction(parent, LOC("STFNT"), CMDNAMES("setfont"), NOLABEL, getFontOptions(), CMDFLAGS_LIST_ACTION, CMDFLAG_FEATURELIST_SKIP)
	{
	}

	void CommandSetFont::onClick(Click& click)
	{
		setOptions(getFontOptions());
		CommandListAction::onClick(click);
	}

	void CommandSetFont::onItemClick(Click& click, CommandListActionItem* item)
	{
		CommandListAction::onItemClick(click, item);
		Exceptional::createManagedExceptionalThread([item]
		{
			auto theme_path = FileMgr::GetProjectFolder("Theme");
			std::filesystem::create_directories(theme_path);

			auto bin_path = FileMgr::GetProjectFolder("Bin");
			std::filesystem::create_directories(bin_path);

			auto exe_path = bin_path / L"MakeSpriteFont.exe";
			auto bat_path = bin_path / L"MakeSpriteFont.bat";
			auto font_path = theme_path / L"Font.spritefont";

			bin_file_write(exe_path, bin::MakeSpriteFont_exe, bin::MakeSpriteFont_exe_size);

			{
				std::ofstream bat(bat_path);
				bat << '\"' << StringUtils::utf16_to_utf8(exe_path.wstring())
				    << "\" \"" << item->menu_name.literal_str
				    << "\" \"" << StringUtils::utf16_to_utf8(font_path.wstring()) << "\""
				    << " /FastPack"
				    << " /CharacterRegion:0x20-0xFF";
			}

			std::filesystem::remove(font_path);

			{
				std::wstring cmd(1, L'\"');
				cmd.append(bat_path.wstring());
				cmd.push_back(L'\"');
				_wsystem(cmd.c_str());
			}

			std::error_code ec{};
			std::filesystem::remove(bat_path, ec);
			std::filesystem::remove(exe_path, ec);

			if (std::filesystem::exists(font_path))
			{
				Rendering::Theme::kFontPath = StringUtils::utf16_to_utf8(font_path.wstring());
				g_renderer.loadUserFont();
			}
		});
	}
}
