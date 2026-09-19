#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandFootSpawnAir : public CommandToggle
    {
    public:
        explicit CommandFootSpawnAir(CommandList* const parent)
            : CommandToggle(parent, LIT("Spawn Aircraft In The Air"), { CMDNAME("footspawnair") },
                LIT("Spawns aircraft above the ground instead of on it."))
        {
        }
    };
}
