#pragma once

#include "Commands/Widgets/CommandSearch.hpp"

#include "Commands/Widgets/CommandLink.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Util/StringUtils.hpp"

namespace Stand
{
	template <bool subfolders>
	class CommandSearchLiterals : public CommandSearch
	{
	public:
		explicit CommandSearchLiterals(CommandList* const parent, std::vector<CommandName>&& command_names)
			: CommandSearch(parent, LOC("SRCH"), std::move(command_names), NOLABEL, SEARCH_SIMPLIFIED)
		{
		}

		void doSearch(std::string&& arg) final
		{
			recursivelySearch(parent, arg);
		}

	protected:
		void recursivelySearch(CommandList* list, const std::string& arg)
		{
			for (const auto& child : list->children)
			{
				auto* phys = child->as<CommandPhysical>();
				if (phys->getMenuName().getLiteralUtf8().empty())
				{
					continue;
				}
				if (subfolders && child->isList())
				{
					recursivelySearch(child->as<CommandList>(), arg);
				}
				else
				{
					if (StringUtils::simplifyCommandName(phys->getMenuName().getLiteralUtf8()).find(arg) != std::string::npos)
					{
						createChild<CommandLink>(child.get());
					}
				}
			}
		}
	};
}
