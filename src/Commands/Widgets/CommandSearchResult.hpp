#pragma once

#include "Commands/Widgets/CommandPhysical.hpp"

namespace Stand
{
    class CommandSearchResult : public CommandPhysical
    {
    public:
        CommandPhysical* const target;

        explicit CommandSearchResult(CommandList* parent, CommandPhysical* target, bool show_address_in_corner = false)
            : CommandPhysical(COMMAND_LINK, parent, Label(target->menu_name), {}, Label(target->help_text), CMDFLAG_TEMPORARY),
              target(target)
        {
        }

        void onClick(Click& click) override
        {
            target->onClick(click);
        }
    };
}
