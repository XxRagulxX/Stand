#pragma once
#include <cmath>
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/World/Places/TpUtil.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    class CommandWaypointPortal : public CommandToggle
    {
    private:
        Vector3 m_anchorPos{};
        float m_anchorHeading{};
        bool m_ticking = false;

        static float dist3D(const Vector3& a, const Vector3& b)
        {
            float dx = b.x - a.x, dy = b.y - a.y, dz = b.z - a.z;
            return std::sqrt(dx * dx + dy * dy + dz * dz);
        }

        static Vector3 rotToDir(float pitch_deg, float yaw_deg)
        {
            float p = pitch_deg * 3.14159265f / 180.0f;
            float y = yaw_deg   * 3.14159265f / 180.0f;
            return { std::cos(p) * -std::sin(y), std::cos(p) * std::cos(y), std::sin(p) };
        }

        void drawPortal() const
        {
            for (int i = 0; i < 360; i++)
            {
                auto from = rotToDir(float(i),     m_anchorHeading);
                auto to   = rotToDir(float(i + 1), m_anchorHeading);
                GRAPHICS::DRAW_LINE(
                    m_anchorPos.x + from.x * 10.0f, m_anchorPos.y + from.y * 10.0f, m_anchorPos.z + from.z * 10.0f,
                    m_anchorPos.x + to.x   * 10.0f, m_anchorPos.y + to.y   * 10.0f, m_anchorPos.z + to.z   * 10.0f,
                    255, 0, 255, 200);
            }
        }

        bool isPlayerInPortal(const Vector3& playerPos) const
        {
            float dx = std::abs(m_anchorPos.x - playerPos.x);
            float dy = std::abs(m_anchorPos.y - playerPos.y);
            float dz = std::abs(m_anchorPos.z - playerPos.z);
            float h  = (m_anchorHeading - 90.0f) * 3.14159265f / 180.0f;
            float sx = std::abs(-std::sin(h));
            float sy = std::abs( std::cos(h));
            return (dx * sx + dy * sy + dz * 0.0f) < 4.5f;
        }

    public:
        explicit CommandWaypointPortal(CommandList* parent)
            : CommandToggle(parent, LIT("Waypoint Portal"), CMDNAMES("wpportals", "wportals", "waypointportals"))
        {}

        ~CommandWaypointPortal() override
        {
            if (m_ticking) CommandTickDispatch::RemoveCommand(this);
        }

        void onEnable(Click& click) final
        {
            if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
        }

        void onDisable(Click& click) final
        {
            if (m_ticking) { CommandTickDispatch::RemoveCommand(this); m_ticking = false; }
        }

        void onTick() override
        {
            if (!HUD::IS_WAYPOINT_ACTIVE())
                return;

            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            bool in_vehicle = veh && ENTITY::DOES_ENTITY_EXIST(veh) && PED::IS_PED_IN_VEHICLE(ped, veh, false);
            int playerEnt = in_vehicle ? veh : ped;
            auto playerPos = ENTITY::GET_ENTITY_COORDS(playerEnt, TRUE);

            if (dist3D(m_anchorPos, playerPos) > 60.0f)
            {
                auto fwd = ENTITY::GET_ENTITY_FORWARD_VECTOR(playerEnt);
                float ox = playerPos.x + fwd.x * 50.0f;
                float oy = playerPos.y + fwd.y * 50.0f;
                float oz = playerPos.z + fwd.z * 50.0f;
                PATH::GET_CLOSEST_VEHICLE_NODE_WITH_HEADING(ox, oy, oz, &m_anchorPos, &m_anchorHeading, 1, 1.0f, 0);
                m_anchorPos.z += 5.0f;
                m_anchorHeading += 90.0f;
            }

            drawPortal();

            if (dist3D(m_anchorPos, playerPos) < 10.0f && isPlayerInPortal(playerPos))
            {
                float speed = in_vehicle ? ENTITY::GET_ENTITY_SPEED(veh) : 0.0f;
                auto blip   = HUD::GET_CLOSEST_BLIP_INFO_ID(HUD::GET_WAYPOINT_BLIP_ENUM_ID());
                auto coords = HUD::GET_BLIP_COORDS(blip);
                TpUtil::teleportWithRedirects(coords.x, coords.y, coords.z, true,
                    [in_vehicle, veh, speed]() {
                        if (in_vehicle)
                            VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
                    });
            }
        }
    };
}
