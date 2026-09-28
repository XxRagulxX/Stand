#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/TeleportTo/CommandListTeleportTo.hpp"
#include "Commands/World/Places/Waypoint/CommandListWaypoint.hpp"
#include "Commands/World/Places/Position/CommandMyPosition.hpp"
#include "Commands/World/Places/CommandRepeatTeleport.hpp"
#include "Commands/World/Places/CommandUndoTp.hpp"
#include "Commands/World/Places/CommandTeleportParticle.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListPlaces : public CommandList
    {
    public:
        CommandListTeleportTo* const teleportTo;
        CommandListWaypoint* const waypoint;
        CommandMyPosition* const position;
        CommandRepeatTeleport* const repeatTp;
        CommandUndoTp* const undoTp;
        CommandTeleportParticle* const tpEffect;

        explicit CommandListPlaces(CommandList* parent)
            : CommandList(parent, LIT("Places")),
              teleportTo(createChild<CommandListTeleportTo>()),
              waypoint(createChild<CommandListWaypoint>()),
              position(createChild<CommandMyPosition>()),
              repeatTp(createChild<CommandRepeatTeleport>()),
              undoTp(createChild<CommandUndoTp>()),
              tpEffect(createChild<CommandTeleportParticle>())
        {
            position->populateChildren();
        }
    };
}
