#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandVehSpawnDrive : public CommandToggle
    {
    public:
        explicit CommandVehSpawnDrive(CommandList* const parent)
            : CommandToggle(parent, LIT("Drive Spawned Vehicles"), { CMDNAME("vehspawndrive") },
                LIT("Instantly puts you into the driver seat of vehicles that you spawn via Stand."), true)
        {
        }
    };
}
