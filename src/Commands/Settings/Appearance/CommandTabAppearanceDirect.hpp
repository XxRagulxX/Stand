#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Settings/Appearance/CommandStreamproof.hpp"
#include "Core/FileMgr.hpp"
#include "Rendering/Theme.hpp"
#include "Menu/Click.hpp"

#include <climits>
#include <filesystem>
#include <shellapi.h>
#include <windows.h>

namespace Stand
{
	class CommandColumns : public CommandSlider
	{
	public:
		explicit CommandColumns(CommandList* const parent)
			: CommandSlider(parent, LIT("Max Columns"), CMDNAMES("columns"),
				NOLABEL, 1, 50, 1, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kColumns = static_cast<int8_t>(value);
		}
	};

	class CommandMenuHeight : public CommandSlider
	{
	public:
		explicit CommandMenuHeight(CommandList* const parent)
			: CommandSlider(parent, LIT("Max Visible Commands"), CMDNAMES("menuheight"),
				NOLABEL, 3, 255, 20, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kMenuHeight = static_cast<uint8_t>(value);
		}
	};

	class CommandListWidth : public CommandSlider
	{
	public:
		explicit CommandListWidth(CommandList* const parent)
			: CommandSlider(parent, LIT("List Width"), CMDNAMES("listwidth"),
				NOLABEL, 0, SHRT_MAX, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kListWidth = static_cast<int16_t>(value);
		}
	};

	class CommandListHeight : public CommandSlider
	{
	public:
		explicit CommandListHeight(CommandList* const parent)
			: CommandSlider(parent, LIT("List Height"), CMDNAMES("listheight"),
				NOLABEL, 0, SHRT_MAX, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kListHeight = static_cast<int16_t>(value);
		}
	};

	class CommandSpacerSize : public CommandSlider
	{
	public:
		explicit CommandSpacerSize(CommandList* const parent)
			: CommandSlider(parent, LIT("Spacer Size"), CMDNAMES("spacersize"),
				NOLABEL, 0, SHRT_MAX, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kSpacerSize = static_cast<int16_t>(value);
		}
	};

	class CommandSmoothScroll : public CommandSlider
	{
	public:
		explicit CommandSmoothScroll(CommandList* const parent)
			: CommandSlider(parent, LIT("Smooth Scrolling"), CMDNAMES("smoothscroll"),
				NOLABEL, 0, 1000, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kSmoothScroll = static_cast<int16_t>(value);
		}
	};

	class CommandBlur : public CommandSlider
	{
	public:
		explicit CommandBlur(CommandList* const parent)
			: CommandSlider(parent, LIT("Background Blur"), CMDNAMES("blur"),
				NOLABEL, 0, 100, 0, 1)
		{
		}

		void onChange(Click& click, int prev_value) override
		{
			Rendering::Theme::kBlur = static_cast<uint8_t>(value);
		}
	};

	class CommandOpenThemeFolder : public CommandPhysical
	{
	public:
		explicit CommandOpenThemeFolder(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LIT("Open Theme Folder"), CMDNAMES("openthemefolder"), NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			// Matches the folder ThemeIcons::LoadImpl() reads from.
			// Stand's own getThemePath() uses %APPDATA%\Stand\Theme\ —
			// we mirror that with %APPDATA%\StandEnhanced\Theme\.
			const auto folder = FileMgr::GetProjectFolder("Theme");
			std::filesystem::create_directories(folder.Path());
			const auto pathStr = folder.Path().string();
			ShellExecuteA(nullptr, "explore", pathStr.c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
		}
	};

	class CommandTabAppearanceDirect : public CommandList
	{
	public:
		CommandColumns* const columns;
		CommandMenuHeight* const menuheight;
		CommandListWidth* const listwidth;
		CommandListHeight* const listheight;
		CommandSpacerSize* const spacersize;
		CommandSmoothScroll* const smoothscroll;
		CommandBlur* const blur;
		CommandOpenThemeFolder* const openThemeFolder;
		CommandStreamproof* const streamproof;

		explicit CommandTabAppearanceDirect()
			: CommandList(nullptr, LIT("Appearance"))
			, columns(createChild<CommandColumns>())
			, menuheight(createChild<CommandMenuHeight>())
			, listwidth(createChild<CommandListWidth>())
			, listheight(createChild<CommandListHeight>())
			, spacersize(createChild<CommandSpacerSize>())
			, smoothscroll(createChild<CommandSmoothScroll>())
			, blur(createChild<CommandBlur>())
			, openThemeFolder(createChild<CommandOpenThemeFolder>())
			, streamproof(createChild<CommandStreamproof>())
		{
		}
	};
}

namespace Stand::Features
{
	Stand::CommandTabAppearanceDirect& GetCommandTabAppearanceDirect();
}
