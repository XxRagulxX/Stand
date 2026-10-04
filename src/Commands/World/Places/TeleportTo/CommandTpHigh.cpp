#include "Commands/World/Places/TeleportTo/CommandTpHigh.hpp"

#include "Commands/World/Places/TpUtil.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandTpHigh::CommandTpHigh(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Very High Up"), CMDNAMES("tphigh"))
    {
    }

    void CommandTpHigh::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            TpUtil::teleport_exact(-75.2188f, -818.582f, 2500.0f);
        });
    }
}
