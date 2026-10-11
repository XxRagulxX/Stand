#include "Commands/Widgets/CommandList.hpp"

#include "Menu/Click.hpp"
#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Util/Label.hpp"

#include <functional>

namespace Stand
{
	void CommandList::recursivelyApplyDefaultState()
	{
		for (auto& child : children)
		{
			if (!child->isPhysical())
				continue;
			auto* cmd = static_cast<CommandPhysical*>(child.get());
			if (cmd->supportsSavedState())
			{
				Click click(CLICK_AUTO, TC_SCRIPT_NOYIELD);
				cmd->applyDefaultState();
			}
			if (cmd->isList())
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
}
