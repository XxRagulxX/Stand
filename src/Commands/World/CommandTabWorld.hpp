#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/CommandListPlaces.hpp"
#include "Commands/World/Inhabitants/CommandListInhabitants.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandTabWorld : public CommandList
    {
    public:
        CommandListPlaces* const places;
        CommandListInhabitants* const inhabitants;

        explicit CommandTabWorld()
            : CommandList(nullptr, LIT("World")),
              places(createChild<CommandListPlaces>()),
              inhabitants(createChild<CommandListInhabitants>())
        {
        }
    };
}

namespace Stand::Features
{
    Stand::CommandTabWorld& GetCommandTabWorld();
}
