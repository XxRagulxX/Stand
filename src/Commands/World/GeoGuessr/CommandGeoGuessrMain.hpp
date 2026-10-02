#pragma once
#include "Commands/Widgets/CommandToggle.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/World/GeoGuessr/CommandGeoGuessr.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Util/get_current_time_millis.hpp"
#include <cmath>
#include <cstdlib>

namespace Stand
{
    class CommandGeoGuessrMain : public CommandToggle
    {
    public:
        struct PosRot
        {
            Vector3 pos;
            Vector3 rot;
        };

        inline static PosRot possible_targets[] = {
            // Expert
            { { 1201.0f, -3112.0f, 6.3f },   { 0.0f, 0.0f, -134.0f } },
            { { -183.9f, 51.8f, 71.0f },      { -1.5f, 0.0f, -105.0f } },
            { { -1564.0f, -417.1f, 39.6f },   { -10.0f, 0.0f, -69.0f } },
            { { 225.6f, -102.2f, 87.9f },     { -90.0f, 0.0f, 69.0f } },
            { { -227.0f, 155.4f, 75.0f },     { 2.8f, 0.0f, 75.0f } },
            { { -1221.2f, -1301.28f, 12.3f }, { -3.7f, 0.0f, 63.4f } },
            { { -17.0f, 6465.0f, 32.4f },     { -2.8f, 0.0f, 57.4f } },
            { { -350.9f, 54.8f, 55.5f },      { -10.4f, 0.0f, -118.3f } },
            { { -1548.8f, 132.6f, 59.2f },    { 0.0f, 0.0f, 166.7f } },
            { { -609.8f, -780.f, 26.4f },     { -1.5f, 0.0f, -88.3f } },
            { { -1086.5f, -365.4f, 39.0f },   { 1.4f, 0.0f, 42.8f } },
            { { -1449.7f, 549.0f, 122.0f },   { -8.4f, 0.0f, -16.6f } },
            { { 846.0f, -2501.0f, 41.4f },    { -4.5f, 0.0f, -110.4f } },
            { { -1121.0f, -2818.f, 40.8f },   { -4.6f, 0.0f, 81.3f } },
            // Hardcore
            { { 867.5963f, -2317.6f, 31.2f }, { 0.0f, 0.0f, -17.7f } },
            { { 1903.8f, 3709.0f, 33.7f },    { -16.0f, 0.0f, 28.0f } },
        };

        explicit CommandGeoGuessrMain(CommandList* parent)
            : CommandToggle(parent, LIT("GeoGuessr"), {}, NOLABEL, false,
                CMDFLAGS_TOGGLE & ~CMDFLAG_SUPPORTS_STATE_OPERATIONS)
        {
        }

        void onEnable(Click& click) override
        {
            ensureYieldableScriptThread(click, [this]
            {
                newRound();
                parent->as<CommandGeoGuessr>()->updateChildVisibility(true);
                CommandTickDispatch::AddCommand(this);
            });
        }

        void onDisable(Click& click) override
        {
            ensureScriptThread(click, [this]
            {
                CommandTickDispatch::RemoveCommand(this);
                parent->as<CommandGeoGuessr>()->cleanup();
            });
        }

        void onTick() override
        {
            if (!m_on)
                return;

            auto* geo = parent->as<CommandGeoGuessr>();

            if (geo->scouting)
            {
                if (geo->m_cam != 0)
                {
                    CAMERA::SET_CAM_COORD(geo->m_cam,
                        geo->m_target_pos.x, geo->m_target_pos.y, geo->m_target_pos.z);
                    CAMERA::SET_CAM_ROT(geo->m_cam,
                        geo->m_target_rot.x, geo->m_target_rot.y, geo->m_target_rot.z, 2);
                    CAMERA::SET_CAM_FOV(geo->m_cam, 50.0f);
                    HUD::DISPLAY_HUD(FALSE);
                    PAD::DISABLE_ALL_CONTROL_ACTIONS(2);
                }
                else
                {
                    geo->stopScouting();
                }
            }
            else if (geo->guessed_at != 0)
            {
                if (geo->m_cam == 0)
                {
                    m_on = false;
                    CommandTickDispatch::RemoveCommand(this);
                    geo->cleanup();
                    return;
                }

                if (GET_MILLIS_SINCE(geo->guessed_at) > 2000)
                {
                    geo->guessed_at = 0;
                    geo->cleanupGuess();
                    newRound();
                }
                else
                {
                    float dx = geo->m_target_pos.x - geo->guess.x;
                    float dy = geo->m_target_pos.y - geo->guess.y;
                    float full_dist = sqrtf(dx * dx + dy * dy);
                    float dist = full_dist * 0.5f;

                    float len = sqrtf(dx * dx + dy * dy);
                    float dir_x = (len > 0.001f) ? dx / len : 0.0f;
                    float dir_y = (len > 0.001f) ? dy / len : 0.0f;

                    float cam_x = geo->guess.x + dir_x * dist;
                    float cam_y = geo->guess.y + dir_y * dist;
                    float cam_z = geo->guess.z + dist;

                    CAMERA::SET_CAM_COORD(geo->m_cam, cam_x, cam_y, cam_z);
                    CAMERA::SET_CAM_ROT(geo->m_cam, -90.0f, 0.0f, 0.0f, 2);
                    CAMERA::SET_CAM_FOV(geo->m_cam, 100.0f);
                    HUD::DISPLAY_HUD(FALSE);

                    GRAPHICS::DRAW_LINE(
                        geo->guess.x, geo->guess.y, geo->guess.z + 1.0f,
                        geo->m_target_pos.x, geo->m_target_pos.y, geo->m_target_pos.z + 1.0f,
                        255, 20, 147, 255);

                    GRAPHICS::DRAW_MARKER(
                        1,
                        geo->m_target_pos.x, geo->m_target_pos.y, geo->m_target_pos.z + 1.0f,
                        0.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 0.0f,
                        1.0f, 1.0f, 1.0f,
                        255, 20, 147, 200,
                        FALSE, TRUE, 2, FALSE, nullptr, nullptr, FALSE);

                    const char* msg;
                    if (full_dist < 10.0f)
                    {
                        msg = "You got it!";
                    }
                    else
                    {
                        static char score_buf[64];
                        snprintf(score_buf, sizeof(score_buf), "Your guess is %d metres from the target.", (int)full_dist);
                        msg = score_buf;
                    }

                    HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("CELL_EMAIL_BCON");
                    HUD::SET_TEXT_SCALE(0, 0.5f);
                    HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
                    HUD::SET_TEXT_FONT(4);
                    HUD::SET_TEXT_JUSTIFICATION(0);
                    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(msg);
                    HUD::END_TEXT_COMMAND_DISPLAY_TEXT(0.5f, 0.45f, 0);
                }
            }
        }

    private:
        void newRound()
        {
            constexpr int count = static_cast<int>(sizeof(possible_targets) / sizeof(possible_targets[0]));
            auto* geo = parent->as<CommandGeoGuessr>();

            PosRot next_target;
            do
            {
                next_target = possible_targets[rand() % count];
            } while (next_target.pos.x == geo->m_target_pos.x
                  && next_target.pos.y == geo->m_target_pos.y);

            geo->m_target_pos = next_target.pos;
            geo->m_target_rot = next_target.rot;
            geo->startScouting();
        }
    };
}
