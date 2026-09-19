#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandVehSpawnFront : public CommandToggle
    {
    public:
        explicit CommandVehSpawnFront(CommandList* const parent)
            : CommandToggle(parent, LIT("Spawn In Front"), { CMDNAME("vehspawnfront") },
                LIT("Puts vehicles that you spawn via Stand a few metres in front of you."), true)
        {
        }
    };
}
