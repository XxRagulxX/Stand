#include "Commands/World/Places/Waypoint/CommandWpClipboard.hpp"

#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <sstream>

namespace Stand
{
    CommandWpClipboard::CommandWpClipboard(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Position From Clipboard"), CMDNAMES("wpclipboard"))
    {
    }

    void CommandWpClipboard::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            auto text = Rendering::Clipboard::GetText();
            float x, y, z;
            char sep;
            std::istringstream ss(text);
            if (!(ss >> x >> sep >> y >> sep >> z))
                return;
            HUD::SET_NEW_WAYPOINT(x, y);
        });
    }
}
