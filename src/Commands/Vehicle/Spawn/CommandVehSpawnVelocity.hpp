#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandVehSpawnVelocity : public CommandToggle
    {
    public:
        explicit CommandVehSpawnVelocity(CommandList* const parent)
            : CommandToggle(parent, LIT("Keep Velocity"), { CMDNAME("vehspawnvelocity") },
                LIT("Applies your current vehicle's velocity to the newly spawned vehicle."))
        {
        }
    };
}
