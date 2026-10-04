#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/World/Places/TeleportTo/CommandListTeleportTo.hpp"
#include "Commands/World/Places/WaypointTo/CommandListWaypoint.hpp"
#include "Commands/World/Places/Position/CommandMyPosition.hpp"
#include "Commands/World/Places/CommandRepeatTeleport.hpp"
#include "Commands/World/Places/CommandUndoTp.hpp"
#include "Commands/World/Places/CommandTeleportParticle.hpp"
#include "Commands/World/Places/Interiors/CommandListInteriors.hpp"
#include "Commands/World/Places/IPLs/CommandListIPLs.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandListPlaces : public CommandList
    {
    public:
        CommandListTeleportTo* const teleportTo;
        CommandRepeatTeleport* const repeatTp;
        CommandUndoTp* const undoTp;
        CommandListWaypoint* const waypoint;
        CommandMyPosition* const position;
        CommandListInteriors* const interiors;
        CommandListIPLs* const ipls;
        CommandTeleportParticle* const tpEffect;

        explicit CommandListPlaces(CommandList* parent)
            : CommandList(parent, LIT("Places")),
              teleportTo(createChild<CommandListTeleportTo>()),
              repeatTp(createChild<CommandRepeatTeleport>()),
              undoTp(createChild<CommandUndoTp>()),
              waypoint(createChild<CommandListWaypoint>()),
              position(createChild<CommandMyPosition>()),
              interiors(createChild<CommandListInteriors>()),
              ipls(createChild<CommandListIPLs>()),
              tpEffect(createChild<CommandTeleportParticle>())
        {
            position->populateChildren();
        }
    };
}
