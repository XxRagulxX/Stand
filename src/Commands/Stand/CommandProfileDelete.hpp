#pragma once

#include <filesystem>

#include "Commands/Stand/CommandProfile.hpp"
#include "Rendering/StandPort/CommandAction.hpp"

namespace Stand
{
	class CommandProfileDelete : public CommandAction
	{
	public:
		explicit CommandProfileDelete(CommandList* const parent)
			: CommandAction(parent, LOC("DEL"), {}, NOLABEL)
		{
		}

		void onClick(Click& click) final
		{
			std::wstring path = parent->as<CommandProfile>()->getFilePath();
			std::error_code err;
			if (std::filesystem::remove(path, err))
			{
				parent->beGone();
			}
		}
	};
}
