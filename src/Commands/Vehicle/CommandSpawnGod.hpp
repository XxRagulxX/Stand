#pragma once
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    class CommandSpawnGod : public CommandToggle
    {
    public:
        explicit CommandSpawnGod(CommandList* const parent)
            : CommandToggle(parent, LIT("Make Spawned Vehicles Indestructible"), { CMDNAME("spawngod") },
                LIT("Applies godmode to every vehicle you spawn via Stand."))
        {
        }
    };
}
