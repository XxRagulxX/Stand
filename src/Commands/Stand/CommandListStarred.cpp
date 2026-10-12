#include "Commands/Stand/CommandListStarred.hpp"

#include "Commands/Widgets/CommandLink.hpp"
#include "Rendering/Gui.hpp"

namespace Stand
{
	CommandListStarred::CommandListStarred(CommandList* parent)
		: CommandList(parent, LOC("CMDSTARS"), {}, LOC("CMDSTARS_H"))
	{
	}

	void CommandListStarred::onActiveListUpdate()
	{
		if (isCurrentUiList())
		{
			resetChildren();
			recursivelyPopulate(g_gui.root_list.get());
			processChildrenUpdate();
		}
	}

	void CommandListStarred::recursivelyPopulate(CommandList* list)
	{
		for (auto& child : list->children)
		{
			if (!child->isPhysical())
			{
				continue;
			}
			auto* cmd = static_cast<CommandPhysical*>(child.get());
			if (cmd->flags & CMDFLAG_STARRED_FINISHLIST)
			{
				break;
			}
			if (!(cmd->flags & CMDFLAG_TEMPORARY))
			{
				auto i = g_gui.starred_commands.data.find(cmd->getPathConfig());
				if (i != g_gui.starred_commands.data.end() && i->second == "Saved")
				{
					this->createChild<CommandLink>(cmd, true);
				}
			}
			if (cmd->isList())
			{
				std::string path_prefix = cmd->getPathConfig() + ".";
				for (const auto& entry : g_gui.starred_commands.data)
				{
					if (entry.first.length() > path_prefix.length()
						&& entry.first.substr(0, path_prefix.length()) == path_prefix
						)
					{
						recursivelyPopulate(static_cast<CommandList*>(cmd));
						break;
					}
				}
			}
		}
	}
}
