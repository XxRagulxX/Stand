#include "Commands/World/Places/CommandAutoTpWp.hpp"

#include "Commands/World/Places/TpUtil.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    CommandAutoTpWp::CommandAutoTpWp(CommandList* parent)
        : CommandToggle(parent, LIT("Auto Teleport To Waypoints"), CMDNAMES("autotpwp"))
    {
    }

    void CommandAutoTpWp::onChange(Click& click)
    {
        onChangeToggleScriptTickEventHandler(click, [this]() -> bool {
            if (!m_on)
                return false;
            if (!HUD::IS_WAYPOINT_ACTIVE())
                return true;
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
            HUD::SET_WAYPOINT_OFF();
            return true;
        });
    }
}
