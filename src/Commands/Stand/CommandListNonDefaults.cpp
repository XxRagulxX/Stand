#include "Commands/Stand/CommandListNonDefaults.hpp"

#include "Commands/Widgets/CommandLink.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandListNonDefaults::CommandListNonDefaults(CommandList* parent)
		: CommandList(parent, LOC("LNDFT"))
	{
	}

	void CommandListNonDefaults::onActiveListUpdate()
	{
		if (isCurrentUiList())
		{
			if (!active)
			{
				active = true;
				resetChildren();
				std::function<void(CommandList*)> recurse = [&](CommandList* list)
				{
					for (auto& child : list->children)
					{
						if (child->isPhysical())
						{
							auto* cmd = static_cast<CommandPhysical*>(child.get());
							if (cmd->supportsStateOperations()
								&& !(cmd->flags & CMDFLAG_TEMPORARY)
								&& cmd->getState() != cmd->getDefaultState()
								)
							{
								createChild<CommandLink>(cmd, true);
							}
							else if (cmd->isList())
							{
								recurse(static_cast<CommandList*>(cmd));
							}
						}
					}
				};
				recurse(g_gui.root_list.get());
			}
		}
		else
		{
			active = false;
		}
	}
}
