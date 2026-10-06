#include "Commands/Widgets/CommandList.hpp"

#include "Menu/Click.hpp"

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
}
