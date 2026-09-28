#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/CommandListPlaces.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandTabWorld : public CommandList
    {
    public:
        CommandListPlaces* const places;

        explicit CommandTabWorld()
            : CommandList(nullptr, LIT("World")),
              places(createChild<CommandListPlaces>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabWorld& GetCommandTabWorld();
}
