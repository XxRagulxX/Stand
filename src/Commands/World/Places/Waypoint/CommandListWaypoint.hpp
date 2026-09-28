#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/Waypoint/CommandWpObjective.hpp"
#include "Commands/World/Places/Waypoint/CommandListWpStores.hpp"
#include "Commands/World/Places/Waypoint/CommandListWpLandmarks.hpp"
#include "Commands/World/Places/Waypoint/CommandWpClipboard.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListWaypoint : public CommandList
    {
    public:
        CommandWpObjective* const wpObjective;
        CommandListWpStores* const stores;
        CommandListWpLandmarks* const landmarks;
        CommandWpClipboard* const wpClipboard;

        explicit CommandListWaypoint(CommandList* parent)
            : CommandList(parent, LIT("Waypoint On..."), CMDNAMES("wp", "waypoint")),
              wpObjective(createChild<CommandWpObjective>()),
              stores(createChild<CommandListWpStores>()),
              landmarks(createChild<CommandListWpLandmarks>()),
              wpClipboard(createChild<CommandWpClipboard>())
        {
        }
    };
}
