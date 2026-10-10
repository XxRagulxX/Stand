#include "Commands/Widgets/CommandList.hpp"

#include "Menu/Click.hpp"
#include "Rendering/GridItemListGrid.hpp"
#include "Rendering/Gui.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Util/Label.hpp"

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
}
