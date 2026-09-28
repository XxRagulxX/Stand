#include "Commands/World/Places/TeleportTo/CommandTpClipboard.hpp"

#include "Commands/World/Places/TpUtil.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <sstream>

namespace Stand
{
    CommandTpClipboard::CommandTpClipboard(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Position From Clipboard"), CMDNAMES("tpclipboard"))
    {
    }

    void CommandTpClipboard::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            auto text = Rendering::Clipboard::GetText();
            float x, y, z;
            char sep;
            std::istringstream ss(text);
            if (!(ss >> x >> sep >> y >> sep >> z))
                return;
            TpUtil::DoTeleport(x, y, z);
        });
    }
}
