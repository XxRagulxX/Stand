#pragma once
#include "Commands/Vehicle/ARSpeed/CommandArSpeedColour.hpp"
#include "Commands/Vehicle/ARSpeed/CommandListArSpeed.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridRenderer.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    namespace
    {
        class CommandArSpeed : public CommandToggle
        {
            CommandListArSpeedColour* const m_colour;

        public:
            explicit CommandArSpeed(CommandList* parent, CommandListArSpeedColour* colour)
                : CommandToggle(parent, LIT("AR Speedometer"), CMDNAMES("arspeedometer", "arspeed"),
                    LIT("Gives your character AR glasses running a speedometer app.")),
                  m_colour(colour)
            {}

            void onChange(Click& click) override
            {
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;

                    if (HUD::IS_PAUSE_MENU_ACTIVE()) return true;

                    int ped = PLAYER::GET_PLAYER_PED(-1);
                    Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
                    Vector3 fwd = ENTITY::GET_ENTITY_FORWARD_VECTOR(ped);

                    float wx = pos.x + fwd.x * 0.5f;
                    float wy = pos.y + fwd.y * 0.5f;
                    float wz = pos.z + 0.7f;

                    float sx, sy;
                    if (!GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(wx, wy, wz, &sx, &sy))
                        return true;

                    int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
                    float spd_ms = ENTITY::GET_ENTITY_SPEED(veh ? veh : ped);
                    float spd_kph = spd_ms * 3.6f;

                    char buf[32];
                    snprintf(buf, sizeof(buf), "%.0f km/h", spd_kph);

                    if (static_cast<CommandListArSpeed*>(parent)->m_use_dx)
                    {
                        DirectX::XMFLOAT4 col{
                            m_colour->m_r / 255.f,
                            m_colour->m_g / 255.f,
                            m_colour->m_b / 255.f,
                            m_colour->m_a / 255.f
                        };
                        Rendering::GridRenderer::QueueText3D(sx, sy, buf, col, 1.0f);
                    }
                    else
                    {
                        HUD::SET_TEXT_FONT(7);
                        HUD::SET_TEXT_SCALE(0.f, 0.45f);
                        HUD::SET_TEXT_COLOUR(m_colour->m_r, m_colour->m_g, m_colour->m_b, m_colour->m_a);
                        HUD::SET_TEXT_CENTRE(TRUE);
                        HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
                        HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buf);
                        HUD::END_TEXT_COMMAND_DISPLAY_TEXT(sx, sy, 0);
                    }

                    return true;
                });
            }
        };
    }
}
