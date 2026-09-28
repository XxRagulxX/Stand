#include "Commands/World/Places/Position/CommandSavePosWp.hpp"

#include "Game/BlipSprite.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace Stand
{
    CommandSavePosWp::CommandSavePosWp(CommandList* parent)
        : CommandSavePos(parent, LIT("Save Waypoint Position"), CMDNAMES("savewppos", "savewpcoords", "savewaypointpos", "savewaypointcoords"))
    {
    }

    bool CommandSavePosWp::waypointExists() const
    {
        Blip blip = HUD::GET_FIRST_BLIP_INFO_ID(static_cast<int>(BlipSprite::RADAR_WAYPOINT));
        return HUD::DOES_BLIP_EXIST(blip);
    }

    std::string CommandSavePosWp::getPos() const
    {
        Blip blip = HUD::GET_FIRST_BLIP_INFO_ID(static_cast<int>(BlipSprite::RADAR_WAYPOINT));
        auto coords = HUD::GET_BLIP_COORDS(blip);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << coords.x << ", " << coords.y << ", " << coords.z;
        return oss.str();
    }

    void CommandSavePosWp::onClick(Click& click)
    {
        click.ensureScriptThread([this](Click& click) {
            if (!waypointExists())
            {
                click.setResponse(LIT("No waypoint found."));
                return;
            }
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            saveToFile(click, std::to_wstring(ms));
        });
    }

    void CommandSavePosWp::onCommand(Click& click, std::wstring& args)
    {
        if (args.empty())
            return;
        std::wstring name = args;
        args.clear();
        click.ensureScriptThread([this, name = std::move(name)](Click& click) {
            if (!waypointExists())
            {
                click.setResponse(LIT("No waypoint found."));
                return;
            }
            saveToFile(click, name);
        });
    }
}
