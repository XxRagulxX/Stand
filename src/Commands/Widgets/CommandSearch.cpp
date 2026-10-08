#include "Commands/Widgets/CommandSearch.hpp"

#include "Commands/Widgets/CommandSearchResult.hpp"
#include "Util/Label.hpp"
#include "lib/soup/unicode.hpp"

#include <algorithm>
#include <cctype>
#include <string>

namespace Stand
{
    CommandSearch::CommandSearch(CommandList* const parent, Label&& menu_name, std::vector<CommandName>&& command_names, Label&& help_text, const search_flags_t search_flags, const commandflags_t flags)
        : CommandList(parent, std::move(menu_name), std::move(command_names), std::move(help_text), flags, COMMAND_LIST_SEARCH), og_menu_name(this->menu_name), search_flags(search_flags)
    {
    }

    Label CommandSearch::getActivationName() const
    {
        return getActivationNameImplCombineWithParent(": ");
    }

    std::string CommandSearch::getCommandSyntax() const
    {
        return std::string("Command: ") + std::string(command_names[0].begin(), command_names[0].end()) + " <query>";
    }

    void CommandSearch::onClick(Click& click)
    {
        return CommandPhysical::onClick(click);
    }

    void CommandSearch::onCommand(Click& click, std::wstring& args)
    {
        std::string arg = soup::unicode::utf16_to_utf8(args);
        args.clear();

        const bool search_arg_can_be_empty = (arg == "*");
        std::string search_arg{};
        if (!search_arg_can_be_empty)
        {
            search_arg = arg;
            if (search_flags & SEARCH_SIMPLIFIED)
            {
                std::transform(search_arg.begin(), search_arg.end(), search_arg.begin(), [](unsigned char c){ return std::tolower(c); });
            }
            else if (search_flags & SEARCH_LOWER)
            {
                std::transform(search_arg.begin(), search_arg.end(), search_arg.begin(), [](unsigned char c){ return std::tolower(c); });
            }
        }
        if (search_arg.empty() && !search_arg_can_be_empty)
        {
            return onClick(click);
        }
        resetChildren();
        doSearch(std::move(search_arg));
        if (children.empty())
        {
            click.setResponse(LOC("SRCHNRES"));
            resetMenuName();
            processChildrenUpdate();
        }
        else
        {
            menu_name.setLiteral(std::string(og_menu_name.getLocalisedUtf8()).append(": ").append(arg));
            active = false;
            open(click.thread_context);
            active = true;
        }
    }

    void CommandSearch::checkForMatchByMenuName(const std::string& arg, CommandPhysical* cmd)
    {
        auto name = std::string(cmd->menu_name.getEnglishUtf8());
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
        if (name.find(arg) != std::string::npos)
        {
            createChild<CommandSearchResult>(cmd, true);
        }
    }

    void CommandSearch::resetMenuName()
    {
        setMenuName(Label(og_menu_name));
    }

    void CommandSearch::onActiveListUpdate()
    {
        CommandList::onActiveListUpdate();

        if (active && !isThisOrSublistActiveInMyTabMenu())
        {
            active = false;
            resetMenuName();
            resetChildren();
        }
    }
}
