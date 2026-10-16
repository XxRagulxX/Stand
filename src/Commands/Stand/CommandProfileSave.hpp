#pragma once

#include <filesystem>
#include <fstream>
#include <unordered_map>

#include "lib/soup/cat.hpp"

#include "Commands/Stand/CommandProfile.hpp"
#include "Rendering/StandPort/CommandAction.hpp"
#include "Core/ExceptionHandler.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	class CommandProfileSave : public CommandAction
	{
	public:
		explicit CommandProfileSave(CommandList* const parent)
			: CommandAction(parent, LOC("SAVE"), { CMDNAME("save"), CMDNAME("saveprofile") }, LOC("SSTATE_H2"))
		{
			addSuffixToCommandNames(parent->menu_name.getLiteralForCommandName());
		}

		void onClick(Click& click) final
		{
			click.setNoResponse();
			Exceptional::createManagedExceptionalThread([this]
			{
				std::unordered_map<std::string, std::string> state;
				g_gui.saveStateInMemory(state);
				auto path = parent->as<CommandProfile>()->getFilePath();
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
			});
		}
	};
}
