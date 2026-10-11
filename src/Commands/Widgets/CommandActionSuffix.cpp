#include "Commands/Widgets/CommandActionSuffix.hpp"

#include "Commands/Widgets/CommandListNameshare.hpp"
#include "Util/lang.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	CommandActionSuffix::CommandActionSuffix(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_suffixes, Label&& help_text, commandflags_t flags, const CommandPerm perm, CommandType type)
		: CommandAction(parent, std::move(menu_name), {}, std::move(help_text), flags, perm, {}, type), command_suffixes(std::move(command_suffixes))
	{
		CommandList* list = parent;
		while (!list->isListNameshare())
		{
			list = list->parent;
		}

		setCommandNames(list->as<CommandListNameshare>());
	}

	void CommandActionSuffix::setCommandNames(CommandListNameshare* base)
	{
		for (const auto& command_suffix : this->command_suffixes)
		{
			for (const auto& command_name : base->command_names)
			{
				command_names.emplace_back(std::move(CommandName(command_name).append(command_suffix)));
			}
		}
	}

	void CommandActionSuffix::updateCommandNames()
	{
		command_names.clear();
		setCommandNames(getListNameshare());
	}

	CommandListNameshare* CommandActionSuffix::getListNameshare() const
	{
		CommandList* list = parent;
		while (!list->isListNameshare())
		{
			list = list->parent;
		}
		return list->as<CommandListNameshare>();
	}

	std::string CommandActionSuffix::getCommandSyntax() const
	{
		if (command_suffixes.empty())
		{
			return {};
		}
		CommandListNameshare* const base = getListNameshare();
		if (base->command_names.empty())
		{
			return {};
		}
		std::wstring syntax{ LANG_GET_W("CMD") };
		syntax.append(L": ").append(cmdNameToUtf16(base->command_names.at(0)));
		syntax.push_back(L' ');
		syntax.append(cmdNameToUtf16(command_suffixes.at(0)));
		return StringUtils::utf16_to_utf8(syntax);
	}
}
