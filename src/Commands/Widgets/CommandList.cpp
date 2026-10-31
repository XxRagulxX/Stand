#include "Commands/Widgets/CommandList.hpp"

#include "Menu/Click.hpp"
#include <algorithm>
#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Util/Label.hpp"

#include <functional>

namespace Stand
{
	void CommandList::onClick(Click& click)
	{
		if (click.type == CLICK_MENU)
		{
			g_gui.m_active_list.push_back(this);
			onActiveListUpdate();
		}
		else
			open(click.thread_context);
	}

	void CommandList::recursivelyApplyDefaultState()
	{
		for (auto& child : children)
		{
			if (!child->isPhysical())
				continue;
			auto* cmd = static_cast<CommandPhysical*>(child.get());
			if (cmd->flags & CMDFLAG_STATE_AND_CHECK_FINISHLIST)
				break;
			if (cmd->supportsSavedState())
			{
				Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
				cmd->applyDefaultState();
			}
			if (cmd->isListNonAction())
				static_cast<CommandList*>(cmd)->recursivelyApplyDefaultState();
		}
	}

	void CommandList::resetChildren() noexcept
	{
		for (auto& child : children)
			child->preDelete();
		children.clear();
	}

	void CommandList::processChildrenUpdate()
	{
	}

	void CommandList::fixCursorAndOffset(bool no_padding)
	{
	}

	std::vector<CommandList*> CommandList::s_active_list_update_listeners{};

	void CommandList::registerForActiveListUpdate()
	{
		s_active_list_update_listeners.push_back(this);
	}

	void CommandList::unregisterFromActiveListUpdate()
	{
		auto& v = s_active_list_update_listeners;
		v.erase(std::remove(v.begin(), v.end(), this), v.end());
	}

	void CommandList::onActiveListUpdate()
	{
	}

	bool CommandList::isThisOrSublistActiveInMyTabMenu() const
	{
		const CommandList* current = g_gui.getCurrentUiList();
		while (current != nullptr)
		{
			if (current == this)
				return true;
			if (current->parent == nullptr || !current->parent->isList())
				break;
			current = static_cast<const CommandList*>(current->parent);
		}
		return false;
	}

	void CommandList::open(ThreadContext thread_context)
	{
		g_gui.m_active_list.push_back(this);
		Rendering::MenuNavigation::Push(menu_name.getLocalisedUtf8(),
			&Rendering::GridItemListGrid::GetOrCreate(this));
	}

	void CommandList::goBackIfActive(ThreadContext thread_context)
	{
		if (isCurrentUiList())
		{
			Rendering::MenuNavigation::Pop();
			if (!g_gui.m_active_list.empty())
				g_gui.m_active_list.pop_back();
		}
	}

	bool CommandList::isCurrentUiList() const noexcept
	{
		return g_gui.getCurrentUiList() == this;
	}

	Command* CommandList::resolveChildByMenuName(const Label& label)
	{
		for (auto& child : children)
		{
			if (!child->isPhysical()) continue;
			auto* p = static_cast<CommandPhysical*>(child.get());
			if (p->menu_name == label) return p;
		}
		return nullptr;
	}

	Command* CommandList::recursivelyResolveChildByMenuName(const Label& label)
	{
		for (auto& child : children)
		{
			if (!child->isPhysical()) continue;
			auto* p = static_cast<CommandPhysical*>(child.get());
			if (p->menu_name == label) return p;
			if (p->isList())
			{
				auto* result = static_cast<CommandList*>(p)->recursivelyResolveChildByMenuName(label);
				if (result) return result;
			}
		}
		return nullptr;
	}

	int8_t CommandList::checkCommandNameMatch(const CommandName& prefix, const CommandName& name)
	{
		if (prefix == name)
			return NMT_HIT;
		if (name.size() > prefix.size() && name.compare(0, prefix.size(), prefix) == 0)
			return NMT_GRAZED;
		if (prefix.size() > name.size() && prefix.compare(0, name.size(), name) == 0)
			return NMT_OVERSHOT;
		return NMT_MISSED;
	}

	bool CommandList::hasCommandsWhereCommandNameStartsWith(const CommandName&) const noexcept
	{
		return true;
	}

	bool CommandList::isThisOrSublist(const CommandList* list) const
	{
		while (list != nullptr)
		{
			if (list == this)
				return true;
			list = list->parent;
		}
		return false;
	}

	void CommandList::recursiveForEachNonListChild(const std::function<void(Command*)>& func) const
	{
		for (const auto& child : children)
		{
			if (child->isList())
				child->as<CommandList>()->recursiveForEachNonListChild(func);
			else
				func(child.get());
		}
	}

	void CommandList::recursiveForEachNonListChildRootReadLocked(const std::function<void(Command*)>& func) const
	{
		g_gui.root_mtx.lockRead();
		recursiveForEachNonListChild(func);
		g_gui.root_mtx.unlockRead();
	}

	void CommandList::dispatchPreDeleteForAllChildren() const
	{
		for (const auto& child : children)
		{
			child->preDelete();
		}
	}

	void CommandList::resetChildrenWithPreDelete()
	{
		dispatchPreDeleteForAllChildren();
		resetChildren();
	}

	CommandPhysical* CommandList::resolveChildByMenuNameLiteral(const std::string& target) const
	{
		for (const auto& _command : this->children)
		{
			if (!_command->isT<CommandPhysical>())
			{
				continue;
			}
			auto command = _command->as<CommandPhysical>();
			if (command->menu_name.isLiteralString(target) && command->canBeResolved())
			{
				return command;
			}
		}
		return nullptr;
	}

	void CommandList::redirectOpen(Click& click, const CommandPhysical* cmd)
	{
		redirectOpen(click.thread_context, cmd);
	}

	void CommandList::redirectOpen(ThreadContext tc, const CommandPhysical* cmd)
	{
		open(tc);
	}

	void CommandList::redirectOpen(const CommandPhysical* cmd)
	{
		open(TC_OTHER);
	}

	void CommandList::evacuateCursor() {}
	void CommandList::evacuateCursorTab(uint8_t tab_idx) {}
	void CommandList::evacuateCursorWeb() {}
	void CommandList::evacuateCursorStandalone() {}
	void CommandList::evacuateCursorStandaloneNocheck() {}

	void CommandList::recursivelyApplyStateFromMemory(const std::unordered_map<std::string, std::string>& state, const std::string& prefix)
	{
		for (const auto& _command : children)
		{
			if (!_command->isPhysical()) continue;
			auto* cmd = static_cast<CommandPhysical*>(_command.get());
			if (cmd->flags & CMDFLAG_TEMPORARY) continue;
			if (cmd->flags & CMDFLAG_STATE_AND_CHECK_FINISHLIST) break;
			if (cmd->supportsStateOperations() && cmd->supportsSavedState())
			{
				std::string path = std::string(prefix).append(cmd->getNameForConfig());
				auto it = state.find(path);
				Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
				if (it != state.end())
					cmd->setState(click, it->second);
				else
					cmd->applyDefaultState();
			}
			if (cmd->isListNonAction())
			{
				std::string child_prefix = std::string(prefix).append(cmd->getNameForConfig());
				child_prefix.push_back('>');
				static_cast<CommandList*>(cmd)->recursivelyApplyStateFromMemory(state, child_prefix);
			}
		}
	}

	void CommandList::recursivelySaveStateInMemory(std::unordered_map<std::string, std::string>& state, const std::string& prefix) const
	{
		for (const auto& _command : this->children)
		{
			if (!_command->isPhysical())
				continue;
			auto* command = (const CommandPhysical*)_command.get();
			if (command->flags & CMDFLAG_TEMPORARY)
				continue;
			if (command->flags & CMDFLAG_STATE_AND_CHECK_FINISHLIST)
				break;
			if (command->supportsStateOperations() && command->supportsSavedState())
			{
				std::string command_state = command->getState();
				if (command_state != command->getDefaultState())
					state.emplace(std::string(prefix).append(command->getNameForConfig()), command_state);
			}
			if (command->isListNonAction())
			{
				std::string prefix_{ prefix };
				prefix_.append(command->getNameForConfig()).push_back('>');
				((CommandList*)command)->recursivelySaveStateInMemory(state, std::move(prefix_));
			}
		}
	}
}
