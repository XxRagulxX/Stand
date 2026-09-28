#include "Commands/World/Places/CommandTpWp.hpp"

#include "Commands/World/Places/TpUtil.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandTpWp::CommandTpWp(CommandList* parent)
        : CommandPhysical(COMMAND_ACTION, parent, LIT("Waypoint"), CMDNAMES("tpwp"))
    {
    }

    void CommandTpWp::onClick(Click& click)
    {
        click.ensureScriptThread([this] {
            if (!HUD::IS_WAYPOINT_ACTIVE())
                return;
            auto blip = HUD::GET_CLOSEST_BLIP_INFO_ID(HUD::GET_WAYPOINT_BLIP_ENUM_ID());
            auto raw = HUD::GET_BLIP_COORDS(blip);
            bool in_vehicle = TpUtil::GetPlayerVehicle() != 0;
            auto tp = TpUtil::FollowRedirects(raw.x, raw.y, raw.z, true, in_vehicle);
            if (!tp.z_exact)
            {
                Vector3 vec{tp.x, tp.y, tp.z};
                TpUtil::ResolveZCoordinate(vec);
                tp.x = vec.x; tp.y = vec.y; tp.z = vec.z;
            }
            TpUtil::DoTeleport(tp.x, tp.y, tp.z);
        });
    }
}
