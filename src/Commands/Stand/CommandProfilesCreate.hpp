#pragma once

#include <filesystem>
#include <fstream>
#include <unordered_map>

#include "lib/soup/cat.hpp"

#include "Commands/Widgets/CommandActionName.hpp"
#include "Commands/Widgets/CommandListRefreshable.hpp"
#include "Core/ExceptionHandler.hpp"
#include "Rendering/Gui.hpp"
#include "Util/get_appdata_path.hpp"

namespace Stand
{
	class CommandProfilesCreate : public CommandActionName
	{
	public:
		explicit CommandProfilesCreate(CommandList* const parent)
			: CommandActionName(parent, LOC("NEWPRFL"), { CMDNAME("newprofile"), CMDNAME("createprofile") }, LOC("NEWPRFL_H"), CMDFLAGS_ACTION_NAME | CMDFLAG_STATE_AND_CHECK_FINISHLIST)
		{
		}

		void onCommand(Click& click, std::wstring& args) final
		{
			if (!onCommandValidateFileName(click, args))
				return;
			click.setNoResponse();
			Exceptional::createManagedExceptionalThread([this, args{ std::move(args) }]() mutable
			{
				std::unordered_map<std::string, std::string> state;
				g_gui.saveStateInMemory(state);
				auto path = get_appdata_path(std::wstring(L"Profiles\\").append(args).append(L".txt"));
				std::filesystem::create_directories(std::filesystem::path(path).parent_path());
				std::ofstream file_out(path);
				for (const auto& [k, v] : state)
				{
					std::string key = k;
					std::string value = v;
					soup::cat::encodeName(key);
					soup::cat::encodeValue(value);
					file_out << key << ": " << value << "\n";
				}
				static_cast<CommandListRefreshable*>(parent)->refresh();
			});
			args.clear();
		}
	};
}
