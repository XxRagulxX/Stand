#pragma once
#include "Game/typedecl.hpp"

namespace Stand
{
    class AllEntitiesEveryTick
    {
    public:
        inline static bool dedsec_mode = false;
        inline static int dedsec_deselect_delay = 0;
        inline static bool dedsec_passive_reticle = true;
        inline static bool dedsec_passive_line = true;
        inline static bool dedsec_passive_box = false;
        inline static bool dedsec_active_reticle = true;
        inline static bool dedsec_active_line = true;
        inline static bool dedsec_active_box = true;
        inline static int dedsec_r = 255;
        inline static int dedsec_g = 0;
        inline static int dedsec_b = 255;
        inline static int dedsec_a = 255;
        inline static int dedsec_h = 300;
        inline static int dedsec_s = 100;
        inline static int dedsec_v = 100;
        inline static int dedsec_rainbow = 0;
        inline static int phone_input_delay = 200;

        inline static punishment_t npc_existence_punishments = 0;
        inline static float npc_punishable_proximity = 10.0f;
        inline static punishment_t npc_proximity_punishments = 0;
        inline static punishment_t npc_hostility_punishments = 0;
        inline static bool npc_hostility_include_everyone = false;
        inline static bool npc_hostility_include_everyone_in_missions = false;
        inline static bool npc_hostility_include_friends = false;
        inline static bool npc_hostility_include_passengers = false;
        inline static bool npc_hostility_include_crew = false;
        inline static bool npc_hostility_include_org = false;
        inline static bool npc_needs_to_aim_at_user = true;
        inline static punishment_t npc_aim_punishments = 0;
        inline static bool player_aim_exclude_friends = true;
        inline static bool player_aim_exclude_crew = false;
        inline static bool player_aim_exclude_stand_users = false;
        inline static bool player_aim_exclude_org = false;
        inline static bool player_needs_to_aim_at_user = true;
        inline static punishment_t player_aim_punishments = 0;
        inline static bool npc_bone_esp = false;
        inline static bool npc_bone_esp_exclude_dead = false;
        inline static int npc_esp_colour_r = 255;
        inline static int npc_esp_colour_g = 0;
        inline static int npc_esp_colour_b = 255;
        inline static bool player_esp_bone = false;
        inline static bool player_esp_name = false;
        inline static int  player_esp_name_max_dist = 1000;
        inline static bool player_esp_box = false;
        inline static int  player_esp_box_max_dist = 1000;
        inline static bool player_esp_line = false;
        inline static int  player_esp_line_max_dist = 1000;
        inline static int  player_esp_colour_r = 255;
        inline static int  player_esp_colour_g = 0;
        inline static int  player_esp_colour_b = 255;
        inline static bool player_esp_name_show_tags = true;
        inline static int  player_esp_name_min_scale = 50;
        inline static int  player_esp_name_max_scale = 100;
        inline static bool player_esp_name_invert_scale = true;

        struct PlayerEspTagEntry { int r; int g; int b; bool use; };
        inline static PlayerEspTagEntry player_esp_tag_friend{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_org{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_modder{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_likely_modder{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_invulnerable{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_veh_god{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_otr{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_invisible{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_crew{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_dead{255, 0, 255, false};
        inline static PlayerEspTagEntry player_esp_tag_rc{255, 0, 255, false};
    };
}
