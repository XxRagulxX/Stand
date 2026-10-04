#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/TeleportTo/CommandTpWp.hpp"
#include "Commands/World/Places/TeleportTo/CommandTpObjective.hpp"
#include "Commands/World/Places/TeleportTo/CommandAutoTpWp.hpp"
#include "Commands/World/Places/TeleportTo/CommandWaypointPortal.hpp"
#include "Commands/World/Places/TeleportTo/CommandListSavedPlaces.hpp"
#include "Commands/World/Places/TeleportTo/CommandListStores.hpp"
#include "Commands/World/Places/TeleportTo/CommandListLandmarks.hpp"
#include "Commands/World/Places/TeleportTo/CommandTpHigh.hpp"
#include "Commands/World/Places/TeleportTo/CommandTpClipboard.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListTeleportTo : public CommandList
    {
    public:
        CommandTpWp* const tpWp;
        CommandTpObjective* const tpObjective;
        CommandListSavedPlaces* const savedPlaces;
        CommandListStores* const stores;
        CommandListLandmarks* const landmarks;
        CommandTpHigh* const tpHigh;
        CommandTpClipboard* const tpClipboard;
        CommandAutoTpWp* const autoTpWp;
        CommandWaypointPortal* const waypointPortal;

        explicit CommandListTeleportTo(CommandList* parent)
            : CommandList(parent, LIT("Teleport To...")),
              tpWp(createChild<CommandTpWp>()),
              tpObjective(createChild<CommandTpObjective>()),
              savedPlaces(createChild<CommandListSavedPlaces>()),
              stores(createChild<CommandListStores>()),
              landmarks(createChild<CommandListLandmarks>()),
              tpHigh(createChild<CommandTpHigh>()),
              tpClipboard(createChild<CommandTpClipboard>()),
              autoTpWp(createChild<CommandAutoTpWp>()),
              waypointPortal(createChild<CommandWaypointPortal>())
        {
        }
    };
}
