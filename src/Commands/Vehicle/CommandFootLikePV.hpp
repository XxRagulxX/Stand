#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandFootLikePV : public CommandToggle
    {
    public:
        explicit CommandFootLikePV(CommandList* const parent)
            : CommandToggle(parent, LIT("Spawn Like Personal Vehicle"), { CMDNAME("footlikepv") },
                LIT("Spawns vehicles on the nearest road node, the way a personal vehicle would appear."))
        {
        }
    };
}
