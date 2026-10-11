#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Online/CommandName.hpp"
#include "Game/typedecl.hpp"

#include <functional>
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
		enum NMT : int8_t
		{
			NMT_MISSED = 0,
			NMT_GRAZED,
			NMT_HIT,
			NMT_OVERSHOT,
		};

		std::vector<std::unique_ptr<Command>> children;
		cursor_t m_cursor = 0;
		cursor_t m_offset = 0;
		ListIndicatorType indicator_type = LISTINDICATOR_ARROW;
		int dividers = 0;
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

		[[nodiscard]] bool canUpdateCursor() const noexcept { return true; }

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

		void recursivelyApplyDefaultState();

		[[nodiscard]] static int8_t checkCommandNameMatch(const CommandName& prefix, const CommandName& name);

		template <class T = CommandPhysical>
		bool findCommandsWhereCommandNameStartsWith(std::vector<T*>& results, const CommandName& command_name_prefix, CommandPerm perm = COMMANDPERM_ALL, size_t results_limit = 2000) const
		{
			for (auto& _command : this->children)
			{
				if (!_command->isT<T>())
					continue;
				auto* command = reinterpret_cast<T*>(_command.get());
				bool recurse_into = !command->isListAction();
				bool grazed = false;
				for (const auto& command_name : command->command_names)
				{
					auto nmt = checkCommandNameMatch(command_name_prefix, command_name);
					if (nmt != NMT_MISSED)
					{
						if (nmt == NMT_HIT)
						{
							results = { command };
							return true;
						}
						else if (nmt == NMT_GRAZED)
						{
							if (!grazed)
							{
								grazed = true;
								if (results.size() != results_limit)
									results.emplace_back(command);
							}
						}
						else if (nmt == NMT_OVERSHOT)
						{
							recurse_into = true;
						}
					}
				}
				if (command->isList()
					&& recurse_into
					&& command->template as<CommandList>()->hasCommandsWhereCommandNameStartsWith(command_name_prefix)
					&& command->template as<CommandList>()->template findCommandsWhereCommandNameStartsWith<T>(results, command_name_prefix, perm, results_limit)
					)
				{
					return true;
				}
			}
			return false;
		}

		[[nodiscard]] virtual bool hasCommandsWhereCommandNameStartsWith(const CommandName& command_name_prefix) const noexcept;

		void resetChildren() noexcept;
		void processChildrenUpdate();
		void fixCursorAndOffset(bool no_padding = false);
		virtual void onActiveListUpdate();
		[[nodiscard]] bool isThisOrSublistActiveInMyTabMenu() const;
		[[nodiscard]] bool isThisOrSublist(const CommandList* list) const;
		void recursiveForEachNonListChild(const std::function<void(Command*)>& func) const;
		void recursiveForEachNonListChildRootReadLocked(const std::function<void(Command*)>& func) const;

		virtual void onBack(ThreadContext thread_context) {}

		void open(ThreadContext thread_context);
		void goBackIfActive(ThreadContext thread_context);
		[[nodiscard]] bool isCurrentUiList() const noexcept;

		[[nodiscard]] Command* resolveChildByMenuName(const Label& label);
		[[nodiscard]] Command* recursivelyResolveChildByMenuName(const Label& label);
	};

	template<>
	[[nodiscard]] inline bool Command::isT<CommandList>() const noexcept { return isList(); }
}
