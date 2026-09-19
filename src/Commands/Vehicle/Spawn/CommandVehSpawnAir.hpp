#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandVehSpawnAir : public CommandToggle
    {
    public:
        explicit CommandVehSpawnAir(CommandList* const parent)
            : CommandToggle(parent, LIT("Spawn Aircraft In The Air"), { CMDNAME("vehspawnair") },
                LIT("Spawns aircraft above the ground instead of on it."))
        {
        }
    };
}
