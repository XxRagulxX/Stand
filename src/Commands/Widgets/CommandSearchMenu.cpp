#include "Commands/Widgets/CommandSearchMenu.hpp"

#include "Commands/Widgets/CommandSearchResult.hpp"
#include "Rendering/Gui.hpp"
#include "lib/soup/StringMatch.hpp"

namespace Stand
{
    CommandSearchMenu::CommandSearchMenu(CommandList* parent)
        : CommandSearch(parent, LIT("Find Command"), CMDNAMES("search", "findcommand", "findcmd"), NOLABEL, SEARCH_LOWER, CMDFLAGS_SEARCH | CMDFLAG_SEARCH_CHILDRENONLY)
    {
    }

    void CommandSearchMenu::doSearch(std::string&& arg)
    {
        bool no_hiding = false;
        if (arg.substr(0, 4) == "all:")
        {
            no_hiding = true;
            arg.erase(0, 4);
        }
        recursivelySearch(arg, no_hiding, g_gui.root_list.get());
    }

    void CommandSearchMenu::recursivelySearch(const std::string& arg, bool no_hiding, CommandList* list)
    {
        for (auto& _cmd : list->children)
        {
            if (!_cmd->isPhysical())
            {
                continue;
            }
            auto* cmd = (CommandPhysical*)_cmd.get();
            if (cmd->flags & CMDFLAG_SEARCH_FINISHLIST)
            {
                if (!no_hiding)
                {
                    break;
                }
            }
            if (!cmd->canBeResolved() || cmd->isConcealed())
            {
                continue;
            }
            if (no_hiding || !(cmd->flags & CMDFLAG_SEARCH_CHILDRENONLY))
            {
                if (soup::StringMatch::search(arg, cmd->menu_name.getEnglishUtf8())
                    || soup::StringMatch::search(arg, cmd->help_text.getEnglishUtf8()))
                {
                    createChild<CommandSearchResult>(cmd, true);
                }
            }
            if (cmd->isList())
            {
                recursivelySearch(arg, no_hiding, (CommandList*)cmd);
            }
        }
    }
}
