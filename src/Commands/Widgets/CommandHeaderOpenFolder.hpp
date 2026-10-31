#pragma once

#include "Commands/Widgets/CommandPhysical.hpp"
#include "Core/FileMgr.hpp"
#include "Menu/Click.hpp"

#include <filesystem>
#include <shellapi.h>
#include <windows.h>

namespace Stand
{
	class CommandHeaderOpenFolder : public CommandPhysical
	{
	public:
		explicit CommandHeaderOpenFolder(CommandList* const parent)
			: CommandPhysical(COMMAND_ACTION, parent, LOC("OPNFLD"),
				{ CMDNAME("openheadersfolder") }, NOLABEL)
		{
		}

		void onClick(Click& click) override
		{
			const auto path = FileMgr::GetProjectFolder("Headers");
			std::filesystem::create_directories(path);
			ShellExecuteW(nullptr, L"explore",
				path.wstring().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
		}
	};
}
