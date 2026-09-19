#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandVehSpawnDelete : public CommandToggle
    {
    public:
        explicit CommandVehSpawnDelete(CommandList* const parent)
            : CommandToggle(parent, LIT("Delete Previous Vehicle"), { CMDNAME("vehspawndelete") },
                LIT("Automatically deletes the previously spawned vehicle when you spawn a new one."))
        {
        }
    };
}
