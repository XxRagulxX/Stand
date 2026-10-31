#include "Commands/Widgets/CommandListActionCategory.hpp"

#include "Commands/Widgets/CommandListSelect.hpp"
#include "Menu/Click.hpp"

namespace Stand
{
	void CommandListActionCategory::onClick(Click& click)
	{
		if (!click.isWeb())
		{
			CommandList::onClick(click);

			if (parent->isT<CommandListSelect>())
			{
				if (auto cmd = parent->as<CommandListSelect>()->getCurrentValueCommand())
				{
					if (cmd->parent == this)
					{
						cmd->focusInParent(click.thread_context);
					}
				}
			}
		}
	}
}
