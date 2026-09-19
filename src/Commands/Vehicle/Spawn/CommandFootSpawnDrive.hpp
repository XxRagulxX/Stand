#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandFootSpawnDrive : public CommandToggle
    {
    public:
        explicit CommandFootSpawnDrive(CommandList* const parent)
            : CommandToggle(parent, LIT("Drive Spawned Vehicles"), { CMDNAME("footspawndrive") },
                LIT("Instantly puts you into the driver seat of vehicles that you spawn via Stand."), true)
        {
        }
    };
}
