#include "Commands/Widgets/CommandIssuable.hpp"

namespace Stand
{
	void CommandIssuable::addSuffixToCommandNames(CommandName&& suffix)
	{
		addSuffixToCommandNamesSimple(suffix);
	}

	void CommandIssuable::addSuffixToCommandNamesSimple(const CommandName& suffix)
	{
		for (auto& command_name : command_names)
			command_name.append(suffix);
	}

	std::vector<CommandName> CommandIssuable::getSuffixedCommandNames(const CommandName& suffix) const
	{
		return combineCommandNames(command_names, suffix);
	}

	std::vector<CommandName> CommandIssuable::getSuffixedCommandNames(const std::vector<CommandName>& suffixes) const
	{
		return combineCommandNames(command_names, suffixes);
	}

	void CommandIssuable::combineCommandNames(std::vector<CommandName>& out_command_names, const std::vector<CommandName>& prefixes, const std::vector<CommandName>& suffixes)
	{
		for (const auto& prefix : prefixes)
		{
			for (const auto& suffix : suffixes)
				out_command_names.emplace_back(CommandName(prefix).append(suffix));
		}
	}

	std::vector<CommandName> CommandIssuable::combineCommandNames(const std::vector<CommandName>& prefixes, const std::vector<CommandName>& suffixes)
	{
		std::vector<CommandName> res;
		res.reserve(prefixes.size() * suffixes.size());
		combineCommandNames(res, prefixes, suffixes);
		return res;
	}

	std::vector<CommandName> CommandIssuable::combineCommandNames(const std::vector<CommandName>& prefixes, const CommandName& suffix)
	{
		std::vector<CommandName> res;
		res.reserve(prefixes.size());
		for (const auto& prefix : prefixes)
			res.emplace_back(CommandName(prefix).append(suffix));
		return res;
	}

	std::vector<CommandName> CommandIssuable::combineCommandNames(const CommandName& prefix, const std::vector<CommandName>& suffixes)
	{
		std::vector<CommandName> res;
		res.reserve(suffixes.size());
		for (const auto& suffix : suffixes)
			res.emplace_back(CommandName(prefix).append(suffix));
		return res;
	}
}
