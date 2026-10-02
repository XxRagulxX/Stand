#include "Commands/World/Watch_Dogs/CommandDedsec.hpp"

#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Game/ControllerInputs.hpp"
#include "Menu/Click.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "Util/get_current_time_millis.hpp"

#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEDSEC_INPUT_PHONE (int)ControllerInputs::INPUT_PHONE

namespace Stand
{
    CommandDedsec::CommandDedsec(CommandList* parent)
        : CommandToggle(parent, LIT("Watch_Dogs-Like World Hacking"), CMDNAMES("dedsecmode", "dedsec", "watchdogs"), NOLABEL)
    {
    }

    void CommandDedsec::onEnable(Click& click)
    {
        AllEntitiesEveryTick::dedsec_mode = true;
        CommandTickDispatch::AddCommand(this);
    }

    void CommandDedsec::onDisable(Click& click)
    {
        m_selected_target = 0;
        m_phone_input_start = 0;
        m_last_hack = nullptr;
        m_deselect_delay_start = 0;
        AllEntitiesEveryTick::dedsec_mode = false;
        CommandTickDispatch::RemoveCommand(this);
    }

    void CommandDedsec::hsvToRgb(int h, int s, int v, int& r, int& g, int& b)
    {
        float hf = (float)h;
        float sf = (float)s / 100.0f;
        float vf = (float)v / 100.0f;
        if (sf == 0.0f)
        {
            int iv = (int)(vf * 255.0f);
            r = g = b = iv;
            return;
        }
        float sector = hf / 60.0f;
        int i = (int)sector;
        float f = sector - (float)i;
        float p = vf * (1.0f - sf);
        float q = vf * (1.0f - sf * f);
        float t = vf * (1.0f - sf * (1.0f - f));
        float rf, gf, bf;
        switch (i % 6)
        {
        case 0:  rf = vf; gf = t;  bf = p;  break;
        case 1:  rf = q;  gf = vf; bf = p;  break;
        case 2:  rf = p;  gf = vf; bf = t;  break;
        case 3:  rf = p;  gf = q;  bf = vf; break;
        case 4:  rf = t;  gf = p;  bf = vf; break;
        default: rf = vf; gf = p;  bf = q;  break;
        }
        r = (int)(rf * 255.0f);
        g = (int)(gf * 255.0f);
        b = (int)(bf * 255.0f);
    }

    void CommandDedsec::showTutorial(int state)
    {
        const char* msg = nullptr;
        switch (state)
        {
        case 0: msg = "Look for a nearby person or vehicle to target."; break;
        case 1: msg = "Use the middle mouse button or ~INPUT_PHONE~ to show hacking options for your current target."; break;
        case 2: msg = "Rotate the camera to select your desired hack, then release the key to execute it."; break;
        default: return;
        }
        HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
        HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(msg);
        HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, FALSE, FALSE, -1);
    }

    Entity CommandDedsec::findTarget(Ped player_ped) const
    {
        Entity best = 0;
        float best_dist = 1.0f;

        constexpr int kMaxNearby = 64;
        int ped_arr[kMaxNearby * 2 + 2] = {};
        ped_arr[0] = kMaxNearby;
        int ped_count = PED::GET_PED_NEARBY_PEDS(player_ped, ped_arr, -1);
        for (int i = 0; i < ped_count; i++)
        {
            Ped p = ped_arr[i * 2 + 2];
            if (p == 0 || p == player_ped) continue;
            if (PED::IS_PED_IN_ANY_VEHICLE(p, FALSE)) continue;
            Vector3 pos = ENTITY::GET_ENTITY_COORDS(p, FALSE);
            float sx = 0.0f, sy = 0.0f;
            if (!GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(pos.x, pos.y, pos.z, &sx, &sy)) continue;
            if (sx < 0.3f || sx > 0.7f || sy < 0.3f || sy > 0.7f) continue;
            float dist = fabsf(0.5f - sx) + fabsf(0.5f - sy);
            if (dist < best_dist)
            {
                best_dist = dist;
                best = p;
            }
        }

        int veh_arr[kMaxNearby * 2 + 2] = {};
        veh_arr[0] = kMaxNearby;
        int veh_count = PED::GET_PED_NEARBY_VEHICLES(player_ped, veh_arr);
        for (int i = 0; i < veh_count; i++)
        {
            Vehicle v = veh_arr[i * 2 + 2];
            if (v == 0) continue;
            Vector3 pos = ENTITY::GET_ENTITY_COORDS(v, FALSE);
            float sx = 0.0f, sy = 0.0f;
            if (!GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(pos.x, pos.y, pos.z, &sx, &sy)) continue;
            if (sx < 0.3f || sx > 0.7f || sy < 0.3f || sy > 0.7f) continue;
            float dist = fabsf(0.5f - sx) + fabsf(0.5f - sy);
            if (dist < best_dist)
            {
                best_dist = dist;
                best = v;
            }
        }

        return best;
    }

    void CommandDedsec::drawBoundingBox(Entity ent, int r, int g, int b, int a) const
    {
        Vector3 fwd{}, right{}, up{}, pos{};
        ENTITY::GET_ENTITY_MATRIX(ent, &fwd, &right, &up, &pos);
        Hash model = ENTITY::GET_ENTITY_MODEL(ent);
        Vector3 mn{}, mx{};
        MISC::GET_MODEL_DIMENSIONS(model, &mn, &mx);

        Vector3 corners[8];
        for (int i = 0; i < 8; i++)
        {
            float lx = (i & 1) ? mx.x : mn.x;
            float ly = (i & 2) ? mx.y : mn.y;
            float lz = (i & 4) ? mx.z : mn.z;
            corners[i].x = pos.x + right.x * lx + fwd.x * ly + up.x * lz;
            corners[i].y = pos.y + right.y * lx + fwd.y * ly + up.y * lz;
            corners[i].z = pos.z + right.z * lx + fwd.z * ly + up.z * lz;
        }

        static constexpr int edges[12][2] = {
            {0,1},{2,3},{4,5},{6,7},
            {0,2},{1,3},{4,6},{5,7},
            {0,4},{1,5},{2,6},{3,7}
        };
        for (auto& e : edges)
        {
            const Vector3& a_v = corners[e[0]];
            const Vector3& b_v = corners[e[1]];
            GRAPHICS::DRAW_LINE(a_v.x, a_v.y, a_v.z, b_v.x, b_v.y, b_v.z, r, g, b, a);
        }
    }

    void CommandDedsec::onTick()
    {
        if (!AllEntitiesEveryTick::dedsec_mode)
            return;

        if (AllEntitiesEveryTick::dedsec_rainbow > 0)
        {
            static time_t s_rainbow_last = 0;
            time_t now = get_current_time_millis();
            if (GET_MILLIS_SINCE(s_rainbow_last) >= (time_t)AllEntitiesEveryTick::dedsec_rainbow)
            {
                s_rainbow_last = now;
                AllEntitiesEveryTick::dedsec_h = (AllEntitiesEveryTick::dedsec_h + 1) % 361;
                hsvToRgb(AllEntitiesEveryTick::dedsec_h, AllEntitiesEveryTick::dedsec_s, AllEntitiesEveryTick::dedsec_v,
                    AllEntitiesEveryTick::dedsec_r, AllEntitiesEveryTick::dedsec_g, AllEntitiesEveryTick::dedsec_b);
            }
        }

        int cr = AllEntitiesEveryTick::dedsec_r;
        int cg = AllEntitiesEveryTick::dedsec_g;
        int cb = AllEntitiesEveryTick::dedsec_b;
        int ca = AllEntitiesEveryTick::dedsec_a;

        Ped player_ped = PLAYER::GET_PLAYER_PED(-1);

        if (m_selected_target != 0 && !ENTITY::DOES_ENTITY_EXIST(m_selected_target))
            m_selected_target = 0;

        Entity dedsec_target = 0;
        if (m_selected_target != 0)
        {
            dedsec_target = m_selected_target;
        }
        else
        {
            dedsec_target = findTarget(player_ped);
        }

        if (dedsec_target == 0)
        {
            m_phone_input_start = 0;
            if (m_tutorial_state == 0)
                showTutorial(0);
            return;
        }

        Vector3 target_pos = ENTITY::GET_ENTITY_COORDS(dedsec_target, FALSE);
        Vector3 player_pos = ENTITY::GET_ENTITY_COORDS(player_ped, TRUE);

        if (m_phone_input_start == -1)
        {
            m_phone_input_start = 0;
        }
        else
        {
            PAD::DISABLE_CONTROL_ACTION(0, DEDSEC_INPUT_PHONE, TRUE);
            if (m_phone_input_start == 0)
            {
                if (PAD::IS_DISABLED_CONTROL_PRESSED(2, DEDSEC_INPUT_PHONE))
                    m_phone_input_start = get_current_time_millis();
            }
            else if (!PAD::IS_DISABLED_CONTROL_PRESSED(2, DEDSEC_INPUT_PHONE))
            {
                if (GET_MILLIS_SINCE(m_phone_input_start) < (time_t)AllEntitiesEveryTick::phone_input_delay)
                {
                    PAD::SET_CONTROL_VALUE_NEXT_FRAME(2, DEDSEC_INPUT_PHONE, 1.0f);
                    m_phone_input_start = -1;
                }
                else
                {
                    m_phone_input_start = 0;
                }
            }
        }

        if (m_selected_target != 0)
        {
            if (AllEntitiesEveryTick::dedsec_active_line)
            {
                GRAPHICS::DRAW_LINE(player_pos.x, player_pos.y, player_pos.z + 1.0f,
                    target_pos.x, target_pos.y, target_pos.z, cr, cg, cb, ca);
            }
            if (AllEntitiesEveryTick::dedsec_active_box)
            {
                drawBoundingBox(dedsec_target, cr, cg, cb, ca);
            }

            if (m_tutorial_state == 1)
            {
                showTutorial(2);
                m_tutorial_state = 2;
            }

            int sw = 1920, sh = 1080;
            GRAPHICS::GET_SCREEN_RESOLUTION(&sw, &sh);
            float hud_x = 1.0f / (float)sw;
            float hud_y = 1.0f / (float)sh;

            Vector3 cam_rot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
            float cam_offset_x;
            {
                cam_offset_x = (cam_rot.z - m_selected_start_rot.z) / 160.0f;
                while (cam_offset_x > 192.0f * hud_x)
                {
                    if (cam_offset_x > 180.0f / 160.0f)
                        m_selected_start_rot.z += 360.0f;
                    else
                        m_selected_start_rot.z += hud_x * 160.0f;
                    cam_offset_x = (cam_rot.z - m_selected_start_rot.z) / 160.0f;
                }
                while (cam_offset_x < -192.0f * hud_x)
                {
                    if (cam_offset_x < -180.0f / 160.0f)
                        m_selected_start_rot.z -= 360.0f;
                    else
                        m_selected_start_rot.z -= hud_x * 160.0f;
                    cam_offset_x = (cam_rot.z - m_selected_start_rot.z) / 160.0f;
                }
            }
            float cam_offset_y;
            {
                cam_offset_y = (cam_rot.x - m_selected_start_rot.x) / 90.0f;
                while (cam_offset_y > 192.0f * hud_y)
                {
                    m_selected_start_rot.x += hud_y * 90.0f;
                    cam_offset_y = (cam_rot.x - m_selected_start_rot.x) / 90.0f;
                }
                while (cam_offset_y < -192.0f * hud_y)
                {
                    m_selected_start_rot.x -= hud_y * 90.0f;
                    cam_offset_y = (cam_rot.x - m_selected_start_rot.x) / 90.0f;
                }
            }

            std::vector<DedsecHack*> hacks;
            bool is_vehicle = ENTITY::IS_ENTITY_A_VEHICLE(dedsec_target) != FALSE;

            if (m_target_frozen)
            {
                if (DedsecHack::unfreeze.enabled) hacks.push_back(&DedsecHack::unfreeze);
            }
            else
            {
                if (DedsecHack::freeze.enabled) hacks.push_back(&DedsecHack::freeze);
            }

            bool is_player_target = false;
            if (is_vehicle)
            {
                if (DedsecHack::destroy.enabled)    hacks.push_back(&DedsecHack::destroy);
                if (DedsecHack::drive.enabled)      hacks.push_back(&DedsecHack::drive);
                if (DedsecHack::enter.enabled)      hacks.push_back(&DedsecHack::enter);
                if (DedsecHack::empty.enabled)      hacks.push_back(&DedsecHack::empty);
                if (DedsecHack::ignite.enabled)     hacks.push_back(&DedsecHack::ignite);
                if (DedsecHack::slingshot.enabled)  hacks.push_back(&DedsecHack::slingshot);
                Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(dedsec_target, -1, FALSE);
                if (driver != 0 && PED::IS_PED_A_PLAYER(driver))
                {
                    if (DedsecHack::menu_player_veh.enabled) hacks.push_back(&DedsecHack::menu_player_veh);
                }
            }
            else
            {
                if (DedsecHack::explode.enabled) hacks.push_back(&DedsecHack::explode);
                if (DedsecHack::disarm.enabled)  hacks.push_back(&DedsecHack::disarm);
                if (PED::IS_PED_A_PLAYER(dedsec_target) != FALSE)
                {
                    is_player_target = true;
                    if (DedsecHack::kill.enabled)        hacks.push_back(&DedsecHack::kill);
                    if (DedsecHack::cage.enabled)        hacks.push_back(&DedsecHack::cage);
                    if (DedsecHack::menu_player.enabled) hacks.push_back(&DedsecHack::menu_player);
                }
                else
                {
                    bool is_dead = ENTITY::IS_ENTITY_DEAD(dedsec_target, FALSE) != FALSE;
                    if (is_dead)
                    {
                        if (DedsecHack::revive.enabled) hacks.push_back(&DedsecHack::revive);
                    }
                    else
                    {
                        if (DedsecHack::burn.enabled) hacks.push_back(&DedsecHack::burn);
                    }
                    if (DedsecHack::cower.enabled) hacks.push_back(&DedsecHack::cower);
                    if (DedsecHack::flee.enabled)  hacks.push_back(&DedsecHack::flee);
                }
            }
            if (!is_player_target)
            {
                if (DedsecHack::del.enabled) hacks.push_back(&DedsecHack::del);
            }

            if (hacks.empty())
            {
                m_selected_target = 0;
                return;
            }

            float step = (2.0f * (float)M_PI) / (float)hacks.size();
            for (int i = 0; i < (int)hacks.size(); i++)
            {
                float angle = (float)i * step;
                float n1 = hacks.size() >= 9 ? 125.0f : 96.0f;
                float n2 = hacks.size() >= 9 ? 10.0f : 0.0f;
                hacks[i]->x = (-sinf(angle) * n1) * hud_x;
                hacks[i]->y = (cosf(angle) * (n1 - n2)) * hud_y;
            }

            bool just_released = PAD::IS_DISABLED_CONTROL_JUST_RELEASED(0, DEDSEC_INPUT_PHONE) != FALSE;
            bool any_active = false;

            for (DedsecHack* hack : hacks)
            {
                float ox = hack->x - cam_offset_x;
                float oy = hack->y - cam_offset_y;
                bool active = (fabsf(ox) < 32.0f * hud_x) && (fabsf(oy) < 32.0f * hud_y);

                if (active)
                {
                    m_last_hack = hack;
                    m_deselect_delay_start = get_current_time_millis();
                }
                else if (m_last_hack != nullptr && hack == m_last_hack
                    && GET_MILLIS_SINCE(m_deselect_delay_start) < (time_t)AllEntitiesEveryTick::dedsec_deselect_delay)
                {
                    active = true;
                }

                if (just_released)
                {
                    if (active)
                    {
                        any_active = true;
                        AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", true);
                        if (hack == &DedsecHack::freeze)   m_target_frozen = true;
                        if (hack == &DedsecHack::unfreeze) m_target_frozen = false;
                        hack->execute(m_selected_target);
                        m_last_hack = nullptr;
                        m_deselect_delay_start = 0;
                    }
                }
                else
                {
                    float draw_x = 0.5f - ox;
                    float draw_y = 0.5f - oy;
                    float draw_w = 64.0f * hud_x;
                    float draw_h = 64.0f * hud_y;
                    GRAPHICS::DRAW_RECT(draw_x, draw_y, draw_w, draw_h,
                        active ? cr : 0, active ? cg : 0, active ? cb : 0, active ? 255 : 127, true);

                    float text_x = draw_x;
                    float text_y = draw_y - 12.0f * hud_y;
                    HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("CELL_EMAIL_BCON");
                    HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
                    HUD::SET_TEXT_FONT(0);
                    HUD::SET_TEXT_SCALE(0, 0.25f);
                    HUD::SET_TEXT_JUSTIFICATION(0);
                    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(hack->label);
                    HUD::END_TEXT_COMMAND_DISPLAY_TEXT(text_x, text_y, 0);
                }
            }

            {
                float name_offset = hacks.size() >= 9 ? 0.475f : 0.5f;
                float name_x = 0.5f + cam_offset_x;
                float name_y = name_offset + cam_offset_y - 164.0f * hud_y;
                const char* name = is_vehicle ? "Vehicle" : "Ped";
                HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("CELL_EMAIL_BCON");
                HUD::SET_TEXT_COLOUR(cr, cg, cb, ca);
                HUD::SET_TEXT_FONT(0);
                HUD::SET_TEXT_SCALE(0, 0.25f);
                HUD::SET_TEXT_JUSTIFICATION(0);
                HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(name);
                HUD::END_TEXT_COMMAND_DISPLAY_TEXT(name_x, name_y, 0);
            }

            if (just_released)
            {
                m_selected_target = 0;
                m_target_frozen = false;
                if (!any_active)
                    AUDIO::PLAY_SOUND_FRONTEND(-1, "CANCEL", "HUD_FRONTEND_DEFAULT_SOUNDSET", true);
            }
        }
        else
        {
            if (AllEntitiesEveryTick::dedsec_passive_line)
            {
                GRAPHICS::DRAW_LINE(player_pos.x, player_pos.y, player_pos.z + 1.0f,
                    target_pos.x, target_pos.y, target_pos.z, cr, cg, cb, ca);
            }
            if (AllEntitiesEveryTick::dedsec_passive_box)
            {
                drawBoundingBox(dedsec_target, cr, cg, cb, ca);
            }

            if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, DEDSEC_INPUT_PHONE))
            {
                AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", true);
                if (m_tutorial_state == 1)
                    m_tutorial_state = 2;
                m_selected_target = dedsec_target;
                m_target_frozen = false;
                m_selected_start_rot = CAMERA::GET_FINAL_RENDERED_CAM_ROT(2);
            }
            else if (m_tutorial_state == 0)
            {
                showTutorial(1);
                m_tutorial_state = 1;
            }
            else if (m_tutorial_state == 1)
            {
                showTutorial(1);
            }
        }
    }
}
