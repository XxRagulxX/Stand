#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"

#include <memory>
#include <utility>
#include <vector>

// Real Stand's CommandList also carries a cursor/scroll offset paired
// with its own GridItemList rendering, a soup::WeakRef "parent for back"
// pointer, web-command state sync, context-menu redirect helpers
// (redirectOpen), and a divider count. None of that is wired to anything
// in this project yet - this is a plain tree container: own the
// children, and provide the createChild<T>()/makeChild<T>() factory
// pattern CommandTabSelf.cpp's own population code is built entirely
// around (list->createChild<CommandFoo>(args...) constructs a
// CommandFoo(this, args...) and takes ownership of it).
namespace Stand
{
	class CommandList : public CommandPhysical
	{
	public:
		std::vector<std::unique_ptr<Command>> children;

		explicit CommandList(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL, commandflags_t flags = CMDFLAGS_LIST, CommandType type = COMMAND_LIST) :
		    CommandPhysical(type, parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags)
		{
		}

		[[nodiscard]] bool isRoot() const noexcept
		{
			return parent == nullptr;
		}

		[[nodiscard]] size_t countVisibleChildren() const
		{
			size_t count = 0;
			for (const auto& child : children)
			{
				if (!child->isConcealed())
					++count;
			}
			return count;
		}

		template<typename T, typename... Args>
		T* createChild(Args&&... args)
		{
			return static_cast<T*>(children.emplace_back(std::make_unique<T>(this, std::forward<Args>(args)...)).get());
		}

		template<typename T, typename... Args>
		std::unique_ptr<T> makeChild(Args&&... args)
		{
			return std::make_unique<T>(this, std::forward<Args>(args)...);
		}
	};
}
