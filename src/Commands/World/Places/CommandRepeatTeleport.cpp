#include "Commands/World/Places/CommandRepeatTeleport.hpp"

#include "Commands/World/Places/TpUtil.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandRepeatTeleport::CommandRepeatTeleport(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Repeat Last Teleport"), CMDNAMES("repeattp"))
    {
    }

    void CommandRepeatTeleport::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            if (!TpUtil::last_tp.has_value())
                return;
            auto& pos = TpUtil::last_tp.value();
            TpUtil::teleport_exact(pos.x, pos.y, pos.z);
        });
    }
}
