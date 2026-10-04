#include "Commands/World/EnhancedOpenWorld/CommandWorldDoors.hpp"

#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/Joaat.hpp"
#include "Util/Label.hpp"

#include <cfloat>
#include <cmath>
#include <stack>
#include <vector>

namespace Stand
{
    struct Pos3
    {
        float x, y, z;
    };

    static float dist3d(const Pos3& a, const Pos3& b)
    {
        float dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
        return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    struct IplSet
    {
        std::vector<const char*> request;
        std::vector<const char*> remove;

        void enable() const
        {
            for (auto* name : request)
                STREAMING::REQUEST_IPL(name);
            for (auto* name : remove)
                STREAMING::REMOVE_IPL(name);
        }

        void disable() const
        {
            for (auto* name : request)
                STREAMING::REMOVE_IPL(name);
            for (auto* name : remove)
                STREAMING::REQUEST_IPL(name);
        }
    };

    static void force_door_open(joaat_t doorHash, joaat_t modelHash, float x, float y, float z)
    {
        if (doorHash == 0)
        {
            joaat_t found = 0;
            if (!OBJECT::DOOR_SYSTEM_FIND_EXISTING_DOOR(x, y, z, modelHash, &found))
                return;
            doorHash = found;
        }
        if (!OBJECT::IS_DOOR_REGISTERED_WITH_SYSTEM(doorHash))
            OBJECT::ADD_DOOR_TO_SYSTEM(doorHash, modelHash, x, y, z, FALSE, FALSE, TRUE, 0);
        OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(doorHash, 0, FALSE, FALSE);
    }

    enum MarkerFlags : uint8_t
    {
        MF_NONE = 0,
        MF_NO_BLIP = 1 << 0,
        MF_SP_ONLY  = 1 << 1,
        MF_NO_MARKER = 1 << 3,
        MF_PROXIMITY_IS_CONTEXT = 1 << 4,
    };

    struct Marker
    {
        Pos3 pos;
        float heading;
        Marker* exit = nullptr;
        const char* name = nullptr;
        uint8_t flags = MF_NONE;
        Blip blip = 0;

        Marker(Pos3 p, float h, Marker* ex = nullptr, const char* n = nullptr, uint8_t f = MF_NONE)
            : pos(p), heading(h), exit(ex), name(n), flags(f) {}

        bool hasBlip() const noexcept { return blip != 0; }
        bool isEntrance() const noexcept { return exit != nullptr; }

        const char* getName() const noexcept
        {
            if (exit)
                return exit->name;
            return name;
        }

        void setBlip(bool toggle)
        {
            if (toggle)
            {
                if (!hasBlip())
                {
                    blip = HUD::ADD_BLIP_FOR_COORD(pos.x, pos.y, pos.z);
                    HUD::SET_BLIP_SPRITE(blip, 130);
                    HUD::SET_BLIP_COLOUR(blip, 8);
                    HUD::SET_BLIP_SCALE(blip, 0.4f);
                    HUD::SET_BLIP_AS_SHORT_RANGE(blip, TRUE);
                    HUD::SHOW_HEIGHT_ON_BLIP(blip, TRUE);
                    HUD::SET_BLIP_NAME_FROM_TEXT_FILE(blip, "MC_GR_PROP_14");
                }
            }
            else
            {
                if (hasBlip())
                {
                    HUD::REMOVE_BLIP(&blip);
                    blip = 0;
                }
            }
        }

        void draw() const
        {
            int r, g, b, a;
            HUD::GET_HUD_COLOUR(16, &r, &g, &b, &a);
            GRAPHICS::DRAW_MARKER(1, pos.x, pos.y, pos.z - 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.7f, 0.5f, r, g, b, 150, false, false, 2, false, nullptr, nullptr, false);
        }

        virtual void onEnter() const {}
        virtual void onLeave() const {}
        virtual void onTick() const {}
    };

    struct MarkerBliponly
    {
        Pos3 pos;
        Blip blip = 0;

        bool hasBlip() const noexcept { return blip != 0; }

        void setBlip(bool toggle)
        {
            if (toggle)
            {
                if (!hasBlip())
                {
                    blip = HUD::ADD_BLIP_FOR_COORD(pos.x, pos.y, pos.z);
                    HUD::SET_BLIP_SPRITE(blip, 130);
                    HUD::SET_BLIP_COLOUR(blip, 8);
                    HUD::SET_BLIP_SCALE(blip, 0.4f);
                    HUD::SET_BLIP_AS_SHORT_RANGE(blip, TRUE);
                    HUD::SHOW_HEIGHT_ON_BLIP(blip, TRUE);
                    HUD::SET_BLIP_NAME_FROM_TEXT_FILE(blip, "MC_GR_PROP_14");
                }
            }
            else
            {
                if (hasBlip())
                {
                    HUD::REMOVE_BLIP(&blip);
                    blip = 0;
                }
            }
        }
    };

    struct MarkerIpl : Marker
    {
        const char* ipl_on;
        const char* ipl_off;

        MarkerIpl(Marker base, const char* on, const char* off = nullptr)
            : Marker(base), ipl_on(on), ipl_off(off) {}

        void onEnter() const override { STREAMING::REQUEST_IPL(ipl_on); }
        void onLeave() const override { STREAMING::REMOVE_IPL(ipl_on); if (ipl_off) STREAMING::REQUEST_IPL(ipl_off); }
    };

    struct MarkerApartment : Marker
    {
        std::vector<joaat_t> exterior_cull_models;

        void onTick() const override
        {
            for (auto hash : exterior_cull_models)
                INTERIOR::ENABLE_EXTERIOR_CULL_MODEL_THIS_FRAME(hash);
        }
    };

    static MarkerBliponly bliponly_jewel{ { -632.06757f, -238.15074f, 38.076836f } };
    static MarkerBliponly bliponly_max_renda_1{ { -584.03186f, -291.6176f, 35.094944f } };
    static MarkerBliponly bliponly_max_renda_2{ { -580.54663f, -280.66177f, 35.314068f } };
    static MarkerBliponly bliponly_eps{ {242.0228f, 360.53632f, 105.733795f} };
    static MarkerBliponly bliponly_janitors{ {-107.09321f, -8.373594f, 70.52472f} };
    static MarkerBliponly bliponly_psb{ {231.8401f, 215.23395f, 106.28019f} };
    static MarkerBliponly bliponly_torture{ {134.20403f, -2203.5354f, 7.186539f} };
    static MarkerBliponly bliponly_fameorshame{ {-254.63133f, -2027.0636f, 29.94654f} };
    static MarkerBliponly bliponly_hospital{ {298.97253f, -584.79126f, 43.26084f} };
    static MarkerBliponly bliponly_vagos{ {-1104.7883f, -1637.6951f, 4.6159596f} };
    static MarkerBliponly bliponly_noose_storage{ {2513.0154f, -318.13214f, 92.9994f} };
    static MarkerBliponly bliponly_omega{ {2331.638f, 2576.2202f, 46.66769f} };
    static MarkerBliponly bliponly_lifeinvader{ {-1082.4508f, -259.72305f, 37.785225f} };
    static MarkerBliponly bliponly_garage_1{ {-1354.984f, -753.3183f, 22.314102f} };
    static MarkerBliponly bliponly_garage_2{ {134.143f, -1054.9728f, 29.192371f} };
    static MarkerBliponly bliponly_garage_3{ {950.80365f, -1698.1649f, 30.085114f} };
    static MarkerBliponly bliponly_fib_lobby{ {105.58902f, -744.4702f, 45.754738f} };
    static MarkerBliponly bliponly_lesters{ {1274.6674f, -1720.8967f, 54.68079f} };
    static MarkerBliponly bliponly_fleeca_1{ {315.32654f, -275.6637f, 53.924805f} };
    static MarkerBliponly bliponly_fleeca_2{ {-2965.8508f, 482.98566f, 15.697028f} };
    static MarkerBliponly bliponly_fleeca_3{ {-349.76382f, -46.553234f, 49.03683f} };
    static MarkerBliponly bliponly_udg_1{ {-73.58169f, -682.04456f, 33.681435f} };
    static MarkerBliponly bliponly_udg_2{ {25.762234f, -664.1427f, 31.628643f} };
    static MarkerBliponly bliponly_garage_sp_1{ {-1074.4863f, -1676.3191f, 4.5787973f} };
    static MarkerBliponly bliponly_garage_sp_2{ {1204.4677f, -3110.2458f, 5.5280185f} };

    static Marker ext_carclub_track{ { -2132.1184f, 1106.0375f, 25.66231f }, -86.0f, nullptr, "LS Car Meet" };
    static Marker ent_carclub_track{ { -2149.1453f, 1106.037f, 28.662169f }, 96.0f, &ext_carclub_track, nullptr, MF_NO_BLIP };

    static Marker ext_server_room{ { 2154.789f, 2920.9937f, -81.07548f }, -90.0f, nullptr, "Server Room" };
    static Marker ent_server_room{ { 2474.0398f, -332.60037f, 92.99268f }, 10.0f, &ext_server_room };

    static Marker ext_ranch{ { 1397.0211f, 1141.81f, 114.33359f }, -87.0f, nullptr, "Life Invader Farm" };
    static Marker ent_ranch{ { 1395.225f, 1141.7489f, 114.63335f }, 95.0f, &ext_ranch };

    static Marker ext_bahamamamas{ { -1387.7535f, -587.6849f, 30.319506f }, -142.0f, nullptr, "Bahama Mamas", MF_PROXIMITY_IS_CONTEXT };
    static Marker ent_bahamamamas{ { -1388.4806f, -586.75726f, 30.218636f }, 34.0f, &ext_bahamamamas, nullptr, MF_PROXIMITY_IS_CONTEXT };

    static Marker ext_therapy{ { -1902.207f, -572.4197f, 19.097233f }, 150.0f, nullptr, "Psychiatrist" };
    static Marker ent_therapy{ { -1913.872f, -574.57324f, 11.435141f }, 150.0f, &ext_therapy };

    static Marker sol_ext{ { -1003.0192f, -477.78375f, 50.027122f }, 117.0f, nullptr, "Solomon's Office" };
    static Marker sol_ent_1{ { -1011.1541f, -479.85855f, 39.970623f }, 122.0f, &sol_ext };
    static Marker sol_ent_2{ { -1007.18646f, -486.62842f, 39.970345f }, 122.0f, &sol_ext };

    static Marker ext_benny{ { -205.37694f, -1315.4332f, 30.8904022f }, 180.0f, nullptr, "Benny's Original Motor Works" };
    static Marker ent_benny{ { -205.91118f, -1310.0565f, 31.295961f }, 0.0f, &ext_benny };

    static Marker ext_chopshop{ { 483.77954f, -1315.965f, 29.200289f }, 118.0f, nullptr, "Hayes Autos", MF_PROXIMITY_IS_CONTEXT };
    static Marker ent_chopshop{ { 485.08405f, -1315.2189f, 29.20865f }, -62.0f, &ext_chopshop, nullptr, MF_PROXIMITY_IS_CONTEXT };

    static Marker ext_tequilala{ { -564.4939f, 277.78168f, 83.13633f }, 270.0f, nullptr, "Tequi-La-La" };
    static Marker ent_tequilala{ { -564.48846f, 275.64615f, 83.10399f }, 180.0f, &ext_tequilala };

    static Marker ext_comedy{ { 382.33365f, -1001.6021f, -99.00011f }, 90.0f, nullptr, "Split Sides Comedy Club" };
    static Marker ent_comedy{ { -430.04214f, 261.3588f, 83.006424f }, 180.0f, &ext_comedy };

    static Marker ext_finbank{ { 0.95314413f, -702.9296f, 16.131021f }, -10.0f, nullptr, "Union Depository" };
    static Marker ent_finbank{ { 10.215674f, -667.76587f, 33.449127f }, 10.0f, &ext_finbank };

    static Marker ext_fib{ { 136.43266f, -760.99554f, 242.15198f }, 160.0f, nullptr, "FIB" };
    static Marker ent_fib{ { 136.43266f, -760.99554f, 45.752052f }, 167.0f, &ext_fib };

    static MarkerIpl ext_morgue{ { {275.74698f, -1361.3822f, 24.5378f}, 50.0f, nullptr, "Morgue" }, "Coroner_Int_on", nullptr };
    static Marker ent_morgue{ { 240.9635f, -1379.0055f, 33.741726f }, 140.0f, &ext_morgue };

    static Marker ext_humane{ { 3627.2808f, 3746.076f, 28.690092f }, 146.0f, nullptr, "Humane Labs" };
    static Marker ent_humane{ { 3621.3074f, 3752.5742f, 28.630383f }, -30.0f, &ext_humane };

    static Marker ext_bunker{ { 894.70526f, -3245.9607f, -98.25847f }, 90.0f, nullptr, "Bunker", MF_SP_ONLY };
    static Marker ent_bunker_1{ { -3032.0483f, 3334.444f, 10.22558f }, -70.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_2{ { 39.539333f, 2930.975f, 55.836376f }, -139.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_3{ { 492.38495f, 3013.8906f, 40.94527f }, -21.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_4{ { 849.24603f, 3021.332f, 41.318073f }, 0.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_5{ { 2110.005f, 3325.6926f, 45.353672f }, 136.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_6{ { 2489.164f, 3161.929f, 48.985325f }, 40.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_7{ { 1801.4082f, 4705.035f, 39.876324f }, 105.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_8{ { -756.44855f, 5943.489f, 19.940222f }, -48.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_9{ { -3158.4153f, 1376.4315f, 16.717169f }, -70.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_10{ { 1572.5353f, 2226.8162f, 78.23683f }, 180.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_bunker_11{ { -389.16873f, 4341.087f, 56.136383f }, -174.0f, &ext_bunker, nullptr, (uint8_t)(MF_SP_ONLY) };

    static Marker ext_vehware{ { 970.86816f, -2987.3152f, -39.64699f }, 180.0f, nullptr, "Vehicle Warehouse", MF_SP_ONLY };
    static Marker ent_vehware_1{ { 804.6517f, -2219.4731f, 29.420275f }, 170.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_vehware_2{ { -72.56631f, -1820.5958f, 26.942152f }, -130.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_vehware_3{ { 1757.9573f, -1646.7076f, 112.64249f }, -170.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_vehware_4{ { 144.34967f, -3006.2542f, 7.030922f }, 0.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_vehware_5{ { 1007.5439f, -1854.4471f, 31.039825f }, 170.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_vehware_6{ { -631.84314f, -1778.9623f, 23.972662f }, 120.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_vehware_7{ { -1151.8207f, -2170.4368f, 13.271458f }, 130.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_vehware_8{ { -514.7716f, -2202.6174f, 6.3940215f }, -40.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };
    static Marker ent_vehware_9{ { 1213.9515f, -1262.769f, 35.22673f }, 90.0f, &ext_vehware, nullptr, (uint8_t)(MF_SP_ONLY) };

    static Marker ext_motel{ {151.40268f, -1007.81415f, -98.99998f}, 0.0f, nullptr, "Motel Room" };
    static Marker ent_motel_1{ {566.3188f, -1778.2246f, 29.353159f}, -26.0f, &ext_motel };
    static Marker ent_motel_2{ {550.2979f, -1775.7576f, 29.312124f}, -115.0f, &ext_motel };
    static Marker ent_motel_3{ {552.2898f, -1771.5514f, 29.312098f}, -115.0f, &ext_motel };
    static Marker ent_motel_4{ {554.73364f, -1766.4291f, 29.312199f}, -115.0f, &ext_motel };
    static Marker ent_motel_5{ {557.997f, -1759.8203f, 29.313643f}, -115.0f, &ext_motel };
    static Marker ent_motel_6{ {561.47504f, -1752.0045f, 29.279654f}, -115.0f, &ext_motel };
    static Marker ent_motel_7{ {560.3833f, -1777.2522f, 33.44705f}, 62.0f, &ext_motel };
    static Marker ent_motel_8{ {559.0805f, -1777.5227f, 33.444f}, -29.0f, &ext_motel };
    static Marker ent_motel_9{ {549.95605f, -1772.9928f, 33.442722f}, -29.0f, &ext_motel };
    static Marker ent_motel_10{ {550.0185f, -1770.6432f, 33.442627f}, -115.0f, &ext_motel };
    static Marker ent_motel_11{ {552.5525f, -1765.29f, 33.442627f}, -115.0f, &ext_motel };
    static Marker ent_motel_12{ {555.4652f, -1758.7952f, 33.442627f}, -115.0f, &ext_motel };
    static Marker ent_motel_14{ {559.20654f, -1750.7898f, 33.442627f}, -115.0f, &ext_motel };
    static Marker ent_motel_15{ {561.8263f, -1747.1501f, 33.442627f}, 148.0f, &ext_motel };

    static Marker* g_markers[] = {
        &ext_carclub_track, &ent_carclub_track,
        &ext_server_room, &ent_server_room,
        &ext_ranch, &ent_ranch,
        &ext_bahamamamas, &ent_bahamamamas,
        &ext_therapy, &ent_therapy,
        &sol_ext, &sol_ent_1, &sol_ent_2,
        &ext_benny, &ent_benny,
        &ext_chopshop, &ent_chopshop,
        &ext_tequilala, &ent_tequilala,
        &ext_comedy, &ent_comedy,
        &ext_finbank, &ent_finbank,
        &ext_fib, &ent_fib,
        &ext_morgue, &ent_morgue,
        &ext_humane, &ent_humane,
        &ext_bunker,
        &ent_bunker_1, &ent_bunker_2, &ent_bunker_3, &ent_bunker_4,
        &ent_bunker_5, &ent_bunker_6, &ent_bunker_7, &ent_bunker_8,
        &ent_bunker_9, &ent_bunker_10, &ent_bunker_11,
        &ext_vehware,
        &ent_vehware_1, &ent_vehware_2, &ent_vehware_3, &ent_vehware_4,
        &ent_vehware_5, &ent_vehware_6, &ent_vehware_7, &ent_vehware_8, &ent_vehware_9,
        &ext_motel,
        &ent_motel_1, &ent_motel_2, &ent_motel_3, &ent_motel_4,
        &ent_motel_5, &ent_motel_6, &ent_motel_7, &ent_motel_8,
        &ent_motel_9, &ent_motel_10, &ent_motel_11, &ent_motel_12,
        &ent_motel_14, &ent_motel_15,
    };

    static IplSet ipl_hospital{ {"RC12B_HospitalInterior"}, {"RC12B_Default"} };
    static IplSet ipl_lost_mc{ {"bkr_bi_hw1_13_int"}, {"hei_bi_hw1_13_door"} };
    static IplSet ipl_fameorshame{ {"sp1_10_real_interior"}, {"sp1_10_fake_interior"} };
    static IplSet ipl_jewel{ {"post_hiest_unload"}, {"jewel2fake"} };
    static IplSet ipl_max_renda{ {"refit_unload"}, {"bh1_16_doors_shut"} };
    static IplSet ipl_fib_lobby{ {"FIBlobby"}, {"FIBlobbyfake"} };
    static IplSet ipl_union_dep{ {"FINBANK"}, {} };
    static IplSet ipl_lifeinvader{ {"facelobby"}, {"facelobbyfake"} };

    static bool isMarkerActive(Marker* m, bool online, const std::stack<Marker*>& interiors)
    {
        if (m->isEntrance())
        {
            if (!interiors.empty() && interiors.top() == m)
                return false;
        }
        else
        {
            if (interiors.empty() || interiors.top()->exit != m)
                return false;
        }
        if (online && (m->flags & MF_SP_ONLY))
            return false;
        return true;
    }

    static void setBliponly(bool toggle, bool online)
    {
        bliponly_jewel.setBlip(toggle);
        bliponly_max_renda_1.setBlip(toggle);
        bliponly_max_renda_2.setBlip(toggle);
        bliponly_eps.setBlip(toggle);
        bliponly_janitors.setBlip(toggle);
        bliponly_psb.setBlip(toggle);
        bliponly_torture.setBlip(toggle);
        bliponly_fameorshame.setBlip(toggle);
        bliponly_hospital.setBlip(toggle);
        bliponly_vagos.setBlip(toggle);
        bliponly_noose_storage.setBlip(toggle);
        bliponly_omega.setBlip(toggle);
        bliponly_lifeinvader.setBlip(toggle);
        bliponly_garage_1.setBlip(toggle);
        bliponly_garage_2.setBlip(toggle);
        bliponly_garage_3.setBlip(toggle);
        if (!toggle || !online)
        {
            bliponly_fib_lobby.setBlip(toggle);
            bliponly_lesters.setBlip(toggle);
            bliponly_fleeca_1.setBlip(toggle);
            bliponly_fleeca_2.setBlip(toggle);
            bliponly_fleeca_3.setBlip(toggle);
            bliponly_udg_1.setBlip(toggle);
            bliponly_udg_2.setBlip(toggle);
            bliponly_garage_sp_1.setBlip(toggle);
            bliponly_garage_sp_2.setBlip(toggle);
        }
    }

    static void disableAllBlips(bool online)
    {
        setBliponly(false, online);
        for (auto* m : g_markers)
            m->setBlip(false);
    }

    static void enableEntranceBlips(bool online, const CommandToggle* blips_cmd, const std::stack<Marker*>& interiors)
    {
        if (blips_cmd->m_on)
        {
            setBliponly(true, online);
            for (auto* m : g_markers)
            {
                m->setBlip(
                    m->isEntrance()
                    && !(m->flags & MF_NO_BLIP)
                    && isMarkerActive(m, online, interiors)
                );
            }
        }
    }

    static void updateBlips(bool online, const CommandToggle* blips_cmd, const std::stack<Marker*>& interiors)
    {
        disableAllBlips(online);
        if (!interiors.empty())
        {
            if (blips_cmd->m_on)
                interiors.top()->exit->setBlip(true);
        }
        else
        {
            enableEntranceBlips(online, blips_cmd, interiors);
        }
    }

    static void pre_tp()
    {
        CAMERA::DO_SCREEN_FADE_OUT(500);
        while (!CAMERA::IS_SCREEN_FADED_OUT())
        {
            PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
            Script::current()->yield();
        }
    }

    static void teleportAndWait(Ped ped, const Pos3& pos, float heading)
    {
        ENTITY::SET_ENTITY_COORDS(ped, pos.x, pos.y, pos.z, FALSE, FALSE, FALSE, TRUE);
        auto viewmode = CAMERA::GET_FOLLOW_PED_CAM_VIEW_MODE();
        CAMERA::SET_FOLLOW_PED_CAM_VIEW_MODE(4);
        Script::current()->yield(500);
        CAMERA::SET_FOLLOW_PED_CAM_VIEW_MODE(viewmode);
        Script::current()->yield(500);
        CAMERA::DO_SCREEN_FADE_IN(500);
        ENTITY::SET_ENTITY_HEADING(ped, heading);
        float rad = heading * 0.017453293f;
        float dx = -sinf(rad);
        float dy = cosf(rad);
        Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, FALSE);
        if (veh != 0)
        {
            TASK::TASK_VEHICLE_DRIVE_TO_COORD(ped, veh, pos.x + dx * 4.0f, pos.y + dy * 4.0f, pos.z, 3.0f, 1500, ENTITY::GET_ENTITY_MODEL(veh), 0, 1.0f, 0.0f);
        }
        else
        {
            TASK::TASK_GO_STRAIGHT_TO_COORD(ped, pos.x + dx * 2.0f, pos.y + dy * 2.0f, pos.z, 1.0f, 1500, heading, 0.5f);
        }
        Script::current()->yield(1500);
    }

    CommandWorldDoors::CommandWorldDoors(CommandList* parent)
        : CommandToggle(parent, LIT("Enhanced Open World"), CMDNAMES("doors", "enhancedopenworld"), LIT("Opens the world up more and allows you to enter many interiors at your own leisure."))
    {
        instance = this;
    }

    void CommandWorldDoors::onEnable(Click& click)
    {
        FiberPool::queueJob([this]
        {
            bool online = NETWORK::NETWORK_IS_SESSION_STARTED() != 0;
            std::stack<Marker*> interiors;

            ipl_hospital.enable();
            ipl_lost_mc.enable();
            ipl_fameorshame.enable();
            ipl_jewel.enable();
            ipl_max_renda.enable();
            ipl_fib_lobby.enable();
            ipl_lifeinvader.enable();
            ipl_union_dep.enable();

            STREAMING::REMOVE_IPL("DT1_03_Shutter");
            STREAMING::REMOVE_IPL("DT1_03_Gr_Closed");

            force_door_open(1538555582, "v_ilev_epsstoredoor"_J, 241.3574f, 361.0488f, 105.8963f);
            force_door_open(-252283844, "v_ilev_janitor_frontdoor"_J, -107.5401f, -9.0258f, 70.6696f);
            force_door_open(963876966, "v_ilev_tort_door"_J, 134.4f, -2204.1f, 7.52f);
            force_door_open(-565026078, "v_ilev_lester_doorfront"_J, 1274.0f, -1721.0f, 55.0f);
            force_door_open(-1788473129, "prop_map_door_01"_J, -1104.66f, -1638.48f, 4.68f);
            force_door_open(-366143778, "hei_v_ilev_bk_gate_pris"_J, 256.31f, 220.66f, 106.43f);

            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(440819155, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(904342475, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-795418380, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-1502457334, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-1994188940, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-1831288286, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-293141277, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(506750037, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(1496005418, 0, FALSE, FALSE);
            OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-1863079210, 0, FALSE, FALSE);

            STREAMING::REQUEST_IPL("gr_case0_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case1_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case2_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case3_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case4_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case5_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case6_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case7_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case9_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case10_bunkerclosed");
            STREAMING::REQUEST_IPL("gr_case11_bunkerclosed");

            enableEntranceBlips(online, blips, interiors);

            while (instance == this && m_on)
            {
                if (!interiors.empty())
                {
                    if (INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::GET_PLAYER_PED(-1)) == 0)
                    {
                        pre_tp();
                        auto* cur = interiors.top();
                        teleportAndWait(PLAYER::GET_PLAYER_PED(-1), cur->pos, cur->heading);
                        cur->onLeave();
                        interiors.pop();
                        enableEntranceBlips(online, blips, interiors);
                    }
                }
                else
                {
                    force_door_open(404057594, "prop_ch3_04_door_01l"_J, 2514.32f, -317.34f, 93.32f);
                    force_door_open(-1417472813, "prop_ch3_04_door_01r"_J, 2512.42f, -319.26f, 93.32f);
                    force_door_open(-1376084479, "prop_ch3_01_trlrdoor_l"_J, 2333.23f, 2574.97f, 47.03f);
                    force_door_open(457472151, "prop_ch3_01_trlrdoor_r"_J, 2329.65f, 2576.64f, 47.03f);
                    force_door_open(-621770121, "v_ilev_bl_doorel_l"_J, -2053.16f, 3239.49f, 30.5f);
                    force_door_open(1018580481, "v_ilev_bl_doorel_r"_J, -2054.39f, 3237.23f, 30.5f);

                    OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(412198396, 0, FALSE, FALSE);
                    OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-1053755588, 0, FALSE, FALSE);
                    OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-93934272, 0, FALSE, FALSE);
                    OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(667682830, 0, FALSE, FALSE);
                    OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(1876735830, 0, FALSE, FALSE);
                    OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-2112857171, 0, FALSE, FALSE);
                    OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-2116116146, 0, FALSE, FALSE);
                    OBJECT::DOOR_SYSTEM_SET_DOOR_STATE(-74083138, 0, FALSE, FALSE);
                }

                Ped ped = PLAYER::GET_PLAYER_PED(-1);
                Vector3 ppos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
                Pos3 player_pos{ ppos.x, ppos.y, ppos.z };

                Marker* closest_marker = nullptr;
                float closest_dist = FLT_MAX;

                for (auto* m : g_markers)
                {
                    if (!isMarkerActive(m, online, interiors))
                        continue;
                    float d = dist3d(player_pos, m->pos);
                    if (d < 100.0f && !(m->flags & MF_NO_MARKER))
                        m->draw();
                    if (d < closest_dist)
                    {
                        closest_dist = d;
                        closest_marker = m;
                    }
                }

                bool cur_online = NETWORK::NETWORK_IS_SESSION_STARTED() != 0;
                if (cur_online != online)
                {
                    online = cur_online;
                    break;
                }

                if (closest_marker && !(closest_marker->flags & MF_NO_MARKER) && !(closest_marker->flags & MF_PROXIMITY_IS_CONTEXT))
                {
                    bool showing_hint = false;
                    if (!m_on)
                    {
                        if (showing_hint)
                            HUD::CLEAR_HELP(TRUE);
                        break;
                    }

                    float threshold = (!(closest_marker->flags & MF_PROXIMITY_IS_CONTEXT) && PED::GET_VEHICLE_PED_IS_IN(ped, FALSE) != 0) ? 3.0f : 1.0f;

                    if (closest_dist < threshold)
                    {
                        bool activated = (closest_marker->flags & MF_PROXIMITY_IS_CONTEXT) != 0
                            || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, 51);

                        if (activated)
                        {
                            pre_tp();
                            Marker* dest;
                            if (closest_marker->isEntrance())
                            {
                                closest_marker->onEnter();
                                interiors.push(closest_marker);
                                dest = closest_marker->exit;

                                FiberPool::queueJob([closest_marker, &interiors]
                                {
                                    while (!interiors.empty() && interiors.top() == closest_marker)
                                    {
                                        closest_marker->onTick();
                                        Script::current()->yield();
                                    }
                                });

                                updateBlips(online, blips, interiors);
                            }
                            else
                            {
                                if (!interiors.empty())
                                {
                                    dest = interiors.top();
                                    dest->onLeave();
                                    interiors.pop();
                                }
                                else
                                {
                                    for (auto* m2 : g_markers)
                                        if (m2->exit == closest_marker)
                                        { dest = m2; break; }
                                }
                                enableEntranceBlips(online, blips, interiors);
                            }
                            teleportAndWait(ped, dest->pos, dest->heading);
                            continue;
                        }
                        else
                        {
                            const char* interior_name = closest_marker->getName();
                            HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
                            if (closest_marker->isEntrance() && interior_name)
                            {
                                HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Press ~INPUT_CONTEXT~ to enter ");
                                HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(interior_name);
                            }
                            else
                            {
                                HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME("Press ~INPUT_CONTEXT~ to exit");
                            }
                            HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, FALSE, TRUE, -1);
                        }
                    }
                }

                if (!m_on)
                    break;

                Script::current()->yield();
            }

            while (!interiors.empty())
            {
                auto* cur = interiors.top();
                ENTITY::SET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(-1), cur->pos.x, cur->pos.y, cur->pos.z, FALSE, FALSE, FALSE, TRUE);
                cur->onLeave();
                interiors.pop();
            }

            disableAllBlips(online);

            ipl_hospital.disable();
            ipl_lost_mc.disable();
            ipl_fameorshame.disable();
            ipl_jewel.disable();
            ipl_max_renda.disable();
            ipl_fib_lobby.disable();
            ipl_lifeinvader.disable();
            ipl_union_dep.disable();

            STREAMING::REMOVE_IPL("gr_case0_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case1_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case2_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case3_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case4_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case5_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case6_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case7_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case9_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case10_bunkerclosed");
            STREAMING::REMOVE_IPL("gr_case11_bunkerclosed");
        });
    }
}
