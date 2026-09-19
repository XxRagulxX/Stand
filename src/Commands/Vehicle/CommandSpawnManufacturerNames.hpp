#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandSpawnManufacturerNames : public CommandToggle
    {
    public:
        explicit CommandSpawnManufacturerNames(CommandList* const parent)
            : CommandToggle(parent, LIT("Manufacturer Names"), { CMDNAME("spawnmanufacturernames") },
                LIT("Show manufacturer name as part of the vehicle name in spawn lists."), true)
        {
        }
    };
}
