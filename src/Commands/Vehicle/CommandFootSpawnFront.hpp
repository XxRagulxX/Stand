#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandFootSpawnFront : public CommandToggle
    {
    public:
        explicit CommandFootSpawnFront(CommandList* const parent)
            : CommandToggle(parent, LIT("Spawn In Front"), { CMDNAME("footspawnfront") },
                LIT("Puts vehicles that you spawn via Stand a few metres in front of you."), true)
        {
        }
    };
}
