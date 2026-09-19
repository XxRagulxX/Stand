#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandFootSpawnDelete : public CommandToggle
    {
    public:
        explicit CommandFootSpawnDelete(CommandList* const parent)
            : CommandToggle(parent, LIT("Delete Previous Vehicle"), { CMDNAME("footspawndelete") },
                LIT("Automatically deletes the previously spawned vehicle when you spawn a new one."))
        {
        }
    };
}
