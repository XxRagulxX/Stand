#pragma once

#include "Commands/Extra/CommandActionScript.hpp"

#include "Util/Util.hpp"

namespace Stand
{
	class CommandOutfitPreset : public CommandActionScript
	{
	private:
		const std::unordered_map<std::string, std::string> data;

	public:
		explicit CommandOutfitPreset(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, std::unordered_map<std::string, std::string>&& data)
			: CommandActionScript(parent, std::move(menu_name), std::move(command_names)), data(std::move(data))
		{
		}

		void onClickScriptThread(Click& click) final
		{
			Util::loadOutfit(this, click, data);
		}
	};
}
