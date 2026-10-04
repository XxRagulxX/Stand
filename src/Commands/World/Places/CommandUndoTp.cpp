#include "Commands/World/Places/CommandUndoTp.hpp"

#include "Commands/World/Places/TpUtil.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandUndoTp::CommandUndoTp(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Undo Teleport"), CMDNAMES("undotp"))
    {
    }

    void CommandUndoTp::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            TpUtil::undo_teleport();
        });
    }
}
