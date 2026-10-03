#include "Commands/World/CommandArWaypoint.hpp"

#include "Game/BlipSprite.hpp"
#include "Scripting/Natives.hpp"

#include <cmath>

namespace Stand
{
    CommandArWaypoint::CommandArWaypoint(CommandList* parent)
        : CommandToggle(parent, LIT("AR Waypoint"), CMDNAMES("arwaypoint", "arwp"),
            LIT("Places a beacon at your waypoints and gives you a way to see the height associated with some waypoints placed via Stand."))
    {
    }

    CommandArWaypoint::~CommandArWaypoint()
    {
        if (m_ticking) CommandTickDispatch::RemoveCommand(this);
    }

    void CommandArWaypoint::onEnable(Click& click)
    {
        CommandTickDispatch::AddCommand(this);
        m_ticking = true;
    }

    void CommandArWaypoint::onDisable(Click& click)
    {
        CommandTickDispatch::RemoveCommand(this);
        m_ticking = false;
    }

    void CommandArWaypoint::onTick()
    {
        if (!HUD::IS_WAYPOINT_ACTIVE())
            return;

        const Blip blip = HUD::GET_FIRST_BLIP_INFO_ID(static_cast<int>(BlipSprite::RADAR_WAYPOINT));
        const Vector3 pos = HUD::GET_BLIP_INFO_ID_COORD(blip);

        constexpr float z_min = -200.0f;
        constexpr float z_max = 10000.0f;
        constexpr float radius = 0.2f;
        constexpr int num_lines = 36;
        constexpr float step = 360.0f / num_lines;
        constexpr float deg2rad = 3.14159265f / 180.0f;

        for (float heading = 0.0f; heading < 360.0f; heading += step)
        {
            const float rad = heading * deg2rad;
            const float px = pos.x + radius * sinf(rad);
            const float py = pos.y + radius * cosf(rad);
            GRAPHICS::DRAW_LINE(px, py, z_min, px, py, z_max, 255, 0, 255, 200);
        }
    }
}
