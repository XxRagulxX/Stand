#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Game/typedecl.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace Stand::Rendering { class Grid; }

namespace Stand
{
	enum ListIndicatorType : uint8_t
	{
		LISTINDICATOR_ARROW = 0,
		LISTINDICATOR_ARROW_IF_CHILDREN,
		LISTINDICATOR_OFF,
		LISTINDICATOR_ON,
	};

	class CommandList : public CommandPhysical
	{
	public:
		std::vector<std::unique_ptr<Command>> children;
		cursor_t m_cursor = 0;
		cursor_t m_offset = 0;
		ListIndicatorType indicator_type = LISTINDICATOR_ARROW;

		explicit CommandList(CommandList* parent, Label&& menu_name, std::vector<CommandName>&& command_names = {}, Label&& help_text = NOLABEL, commandflags_t flags = CMDFLAGS_LIST, CommandType type = COMMAND_LIST) :
		    CommandPhysical(type, parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags)
		{
		}

		virtual bool requiresVehicle() const { return false; }
		virtual const char* vehicleRequiredMessage() const { return "Get your ass in a vehicle :/"; }

		virtual void onBecomesActiveGrid(Rendering::Grid* grid) {}

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
