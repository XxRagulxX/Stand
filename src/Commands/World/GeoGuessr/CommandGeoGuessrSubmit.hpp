#pragma once
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/World/GeoGuessr/CommandGeoGuessr.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Util/get_current_time_millis.hpp"

namespace Stand
{
    class CommandGeoGuessrSubmit : public CommandPhysical
    {
    public:
        explicit CommandGeoGuessrSubmit(CommandList* parent)
            : CommandPhysical(COMMAND_ACTION, parent, LIT("Take Your Guess"), {},
                LIT("Input a position by setting a waypoint, using freecam, or moving/teleporting your character."),
                CMDFLAGS_ACTION | CMDFLAG_CONCEALED)
        {
        }

        void onClick(Click& click) override
        {
            ensureScriptThread(click, [this]
            {
                auto* geo = parent->as<CommandGeoGuessr>();
                if (geo->guessed_at != 0)
                    return;

                Vector3 pos{};
                Blip wp = HUD::GET_FIRST_BLIP_INFO_ID(HUD::GET_WAYPOINT_BLIP_ENUM_ID());
                if (HUD::DOES_BLIP_EXIST(wp))
                    pos = HUD::GET_BLIP_INFO_ID_COORD(wp);
                else
                    pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(-1), FALSE);

                geo->stopScouting();
                geo->startResultCam();
                geo->guessed_at = get_current_time_millis();
                geo->guess = pos;
            });
        }
    };
}
