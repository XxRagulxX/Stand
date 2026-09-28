#include "Commands/World/Places/Waypoint/CommandWpObjective.hpp"

#include "Game/BlipSprite.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandWpObjective::CommandWpObjective(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Objective"), CMDNAMES("wpobjective"))
    {
    }

    void CommandWpObjective::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            static constexpr BlipSprite kSprites[] = {
                BlipSprite::RADAR_LEVEL, BlipSprite::RADAR_HIGHER, BlipSprite::RADAR_LOWER,
                BlipSprite::RADAR_OBJECTIVE_BLUE, BlipSprite::RADAR_OBJECTIVE_GREEN,
                BlipSprite::RADAR_OBJECTIVE_RED, BlipSprite::RADAR_OBJECTIVE_YELLOW,
                BlipSprite::RADAR_CONTRABAND,
                BlipSprite::RADAR_TARGET_A, BlipSprite::RADAR_TARGET_B, BlipSprite::RADAR_TARGET_C,
                BlipSprite::RADAR_TARGET_D, BlipSprite::RADAR_TARGET_E, BlipSprite::RADAR_TARGET_F,
                BlipSprite::RADAR_TARGET_G, BlipSprite::RADAR_TARGET_H,
                BlipSprite::RADAR_PICKUP_MACHINEGUN,
            };
            for (const auto sprite : kSprites)
            {
                Blip blip = HUD::GET_CLOSEST_BLIP_INFO_ID(static_cast<int>(sprite));
                if (!blip)
                    continue;
                auto raw = HUD::GET_BLIP_COORDS(blip);
                HUD::SET_NEW_WAYPOINT(raw.x, raw.y);
                return;
            }
        });
    }
}
