#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandVehLikePV : public CommandToggle
    {
    public:
        explicit CommandVehLikePV(CommandList* const parent)
            : CommandToggle(parent, LIT("Spawn Like Personal Vehicle"), { CMDNAME("vehlikepv") },
                LIT("Spawns vehicles on the nearest road node, the way a personal vehicle would appear."))
        {
        }
    };
}
