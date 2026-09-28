#include "Commands/World/Places/IPLs/CommandListIPLs.hpp"
#include "Commands/World/Places/IPLs/CommandIpl.hpp"
#include "Commands/World/Places/IPLs/CommandNorthYanktonMap.hpp"
#include "Commands/Stand/CommandToggleNoCorrelation.hpp"
#include "Util/Label.hpp"

namespace Stand
{
    static Vector3 v3(float x, float y, float z)
    {
        Vector3 v{};
        v.x = x; v.y = y; v.z = z;
        return v;
    }

    CommandListIPLs::CommandListIPLs(CommandList* parent)
        : CommandList(parent, LIT("IPLs"), CMDNAMES("ipls"), LIT("Allows you to load and unload parts of the world (\"IPLs\") locally."))
    {
        auto tp_toggle = createChild<CommandToggleNoCorrelation>(LIT("Teleport To IPLs"), CMDNAMES("ipltp"), LIT("Will teleport you to IPLs when you enable them below."));

        auto ipl = [&](Label name, std::vector<CommandName> cmds, Vector3 pos,
                       std::vector<const char*> req,
                       std::vector<const char*> rem = {},
                       std::vector<const char*> rem_eo = {},
                       bool mp = false)
        {
            createChild<CommandIpl>(std::move(name), std::move(cmds), pos,
                                    std::move(req), std::move(rem), std::move(rem_eo), mp, tp_toggle);
        };

        ipl(LIT("Cayo Perico"), CMDNAMES("iplcayoperico", "iplperico", "iplcayopercio", "iplpercio"),
            v3(4906.2554f, -4912.7646f, 3.3630776f),
            {"h4_islandx_terrain_01_lod","h4_islandx_terrain_01_slod","h4_islandx_terrain_02","h4_islandx_terrain_02_lod","h4_islandx_terrain_02_slod","h4_islandx_terrain_03","h4_islandx_terrain_03_lod","h4_islandx_terrain_04","h4_islandx_terrain_04_lod","h4_islandx_terrain_04_slod","h4_islandx_terrain_05","h4_islandx_terrain_05_lod","h4_islandx_terrain_05_slod","h4_islandx_terrain_06","h4_mph4_terrain_01_grass_0","h4_mph4_terrain_01_grass_1","h4_mph4_terrain_02_grass_0","h4_mph4_terrain_02_grass_1","h4_mph4_terrain_02_grass_2","h4_mph4_terrain_02_grass_3","h4_mph4_terrain_04_grass_0","h4_mph4_terrain_04_grass_1","h4_mph4_terrain_05_grass_0","h4_mph4_terrain_06_grass_0","h4_islandx_terrain_01","h4_islandx_terrain_06_lod","h4_islandx_terrain_06_slod","h4_islandx_terrain_props_05_a","h4_islandx_terrain_props_05_a_lod","h4_islandx_terrain_props_05_b","h4_islandx_terrain_props_05_b_lod","h4_islandx_terrain_props_05_c","h4_islandx_terrain_props_05_c_lod","h4_islandx_terrain_props_05_d","h4_islandx_terrain_props_05_d_lod","h4_islandx_terrain_props_05_d_slod","h4_islandx_terrain_props_05_e","h4_islandx_terrain_props_05_e_lod","h4_islandx_terrain_props_05_e_slod","h4_islandx_terrain_props_05_f","h4_islandx_terrain_props_05_f_lod","h4_islandx_terrain_props_05_f_slod","h4_islandx_terrain_props_06_a","h4_islandx_terrain_props_06_a_lod","h4_islandx_terrain_props_06_a_slod","h4_islandx_terrain_props_06_b","h4_islandx_terrain_props_06_b_lod","h4_islandx_terrain_props_06_b_slod","h4_islandx_terrain_props_06_c","h4_islandx_terrain_props_06_c_lod","h4_islandx_terrain_props_06_c_slod","h4_mph4_terrain_01","h4_mph4_terrain_01_long_0","h4_mph4_terrain_02","h4_mph4_terrain_03","h4_mph4_terrain_04","h4_mph4_terrain_05","h4_mph4_terrain_06","h4_mph4_terrain_06_strm_0","h4_mph4_terrain_lod","h4_mph4_terrain_occ_00","h4_mph4_terrain_occ_01","h4_mph4_terrain_occ_02","h4_mph4_terrain_occ_03","h4_mph4_terrain_occ_04","h4_mph4_terrain_occ_05","h4_mph4_terrain_occ_06","h4_mph4_terrain_occ_07","h4_mph4_terrain_occ_08","h4_mph4_terrain_occ_09","h4_boatblockers","h4_islandx","h4_islandx_disc_strandedshark","h4_islandx_disc_strandedshark_lod","h4_islandx_disc_strandedwhale","h4_islandx_disc_strandedwhale_lod","h4_islandx_props","h4_islandx_props_lod","h4_islandx_sea_mines","h4_mph4_island","h4_mph4_island_long_0","h4_mph4_island_strm_0","h4_aa_guns","h4_aa_guns_lod","h4_beach","h4_beach_bar_props","h4_beach_lod","h4_beach_party","h4_beach_party_lod","h4_beach_props","h4_beach_props_lod","h4_beach_props_party","h4_beach_props_slod","h4_beach_slod","h4_islandairstrip","h4_islandairstrip_doorsclosed","h4_islandairstrip_doorsclosed_lod","h4_islandairstrip_doorsopen","h4_islandairstrip_doorsopen_lod","h4_islandairstrip_hangar_props","h4_islandairstrip_hangar_props_lod","h4_islandairstrip_hangar_props_slod","h4_islandairstrip_lod","h4_islandairstrip_props","h4_islandairstrip_propsb","h4_islandairstrip_propsb_lod","h4_islandairstrip_propsb_slod","h4_islandairstrip_props_lod","h4_islandairstrip_props_slod","h4_islandairstrip_slod","h4_islandxcanal_props","h4_islandxcanal_props_lod","h4_islandxcanal_props_slod","h4_islandxdock","h4_islandxdock_lod","h4_islandxdock_props","h4_islandxdock_props_2","h4_islandxdock_props_2_lod","h4_islandxdock_props_2_slod","h4_islandxdock_props_lod","h4_islandxdock_props_slod","h4_islandxdock_slod","h4_islandxdock_water_hatch","h4_islandxtower","h4_islandxtower_lod","h4_islandxtower_slod","h4_islandxtower_veg","h4_islandxtower_veg_lod","h4_islandxtower_veg_slod","h4_islandx_barrack_hatch","h4_islandx_barrack_props","h4_islandx_barrack_props_lod","h4_islandx_barrack_props_slod","h4_islandx_checkpoint","h4_islandx_checkpoint_lod","h4_islandx_checkpoint_props","h4_islandx_checkpoint_props_lod","h4_islandx_checkpoint_props_slod","h4_islandx_maindock","h4_islandx_maindock_lod","h4_islandx_maindock_props","h4_islandx_maindock_props_2","h4_islandx_maindock_props_2_lod","h4_islandx_maindock_props_2_slod","h4_islandx_maindock_props_lod","h4_islandx_maindock_props_slod","h4_islandx_maindock_slod","h4_islandx_mansion","h4_islandx_mansion_b","h4_islandx_mansion_b_lod","h4_islandx_mansion_b_side_fence","h4_islandx_mansion_b_slod","h4_islandx_mansion_entrance_fence","h4_islandx_mansion_guardfence","h4_islandx_mansion_lights","h4_islandx_mansion_lockup_01","h4_islandx_mansion_lockup_01_lod","h4_islandx_mansion_lockup_02","h4_islandx_mansion_lockup_02_lod","h4_islandx_mansion_lockup_03","h4_islandx_mansion_lockup_03_lod","h4_islandx_mansion_lod","h4_islandx_mansion_office","h4_islandx_mansion_office_lod","h4_islandx_mansion_props","h4_islandx_mansion_props_lod","h4_islandx_mansion_props_slod","h4_islandx_mansion_slod","h4_islandx_mansion_vault","h4_islandx_mansion_vault_lod","h4_island_padlock_props","h4_mansion_gate_broken","h4_mansion_gate_closed","h4_mansion_remains_cage","h4_mph4_airstrip","h4_mph4_airstrip_interior_0_airstrip_hanger","h4_mph4_beach","h4_mph4_dock","h4_mph4_island_lod","h4_mph4_island_ne_placement","h4_mph4_island_nw_placement","h4_mph4_island_se_placement","h4_mph4_island_sw_placement","h4_mph4_mansion","h4_mph4_mansion_b","h4_mph4_mansion_b_strm_0","h4_mph4_mansion_strm_0","h4_mph4_wtowers","h4_ne_ipl_00","h4_ne_ipl_00_lod","h4_ne_ipl_00_slod","h4_ne_ipl_01","h4_ne_ipl_01_lod","h4_ne_ipl_01_slod","h4_ne_ipl_02","h4_ne_ipl_02_lod","h4_ne_ipl_02_slod","h4_ne_ipl_03","h4_ne_ipl_03_lod","h4_ne_ipl_03_slod","h4_ne_ipl_04","h4_ne_ipl_04_lod","h4_ne_ipl_04_slod","h4_ne_ipl_05","h4_ne_ipl_05_lod","h4_ne_ipl_05_slod","h4_ne_ipl_06","h4_ne_ipl_06_lod","h4_ne_ipl_06_slod","h4_ne_ipl_07","h4_ne_ipl_07_lod","h4_ne_ipl_07_slod","h4_ne_ipl_08","h4_ne_ipl_08_lod","h4_ne_ipl_08_slod","h4_ne_ipl_09","h4_ne_ipl_09_lod","h4_ne_ipl_09_slod","h4_nw_ipl_00","h4_nw_ipl_00_lod","h4_nw_ipl_00_slod","h4_nw_ipl_01","h4_nw_ipl_01_lod","h4_nw_ipl_01_slod","h4_nw_ipl_02","h4_nw_ipl_02_lod","h4_nw_ipl_02_slod","h4_nw_ipl_03","h4_nw_ipl_03_lod","h4_nw_ipl_03_slod","h4_nw_ipl_04","h4_nw_ipl_04_lod","h4_nw_ipl_04_slod","h4_nw_ipl_05","h4_nw_ipl_05_lod","h4_nw_ipl_05_slod","h4_nw_ipl_06","h4_nw_ipl_06_lod","h4_nw_ipl_06_slod","h4_nw_ipl_07","h4_nw_ipl_07_lod","h4_nw_ipl_07_slod","h4_nw_ipl_08","h4_nw_ipl_08_lod","h4_nw_ipl_08_slod","h4_nw_ipl_09","h4_nw_ipl_09_lod","h4_nw_ipl_09_slod","h4_se_ipl_00","h4_se_ipl_00_lod","h4_se_ipl_00_slod","h4_se_ipl_01","h4_se_ipl_01_lod","h4_se_ipl_01_slod","h4_se_ipl_02","h4_se_ipl_02_lod","h4_se_ipl_02_slod","h4_se_ipl_03","h4_se_ipl_03_lod","h4_se_ipl_03_slod","h4_se_ipl_04","h4_se_ipl_04_lod","h4_se_ipl_04_slod","h4_se_ipl_05","h4_se_ipl_05_lod","h4_se_ipl_05_slod","h4_se_ipl_06","h4_se_ipl_06_lod","h4_se_ipl_06_slod","h4_se_ipl_07","h4_se_ipl_07_lod","h4_se_ipl_07_slod","h4_se_ipl_08","h4_se_ipl_08_lod","h4_se_ipl_08_slod","h4_se_ipl_09","h4_se_ipl_09_lod","h4_se_ipl_09_slod","h4_sw_ipl_00","h4_sw_ipl_00_lod","h4_sw_ipl_00_slod","h4_sw_ipl_01","h4_sw_ipl_01_lod","h4_sw_ipl_01_slod","h4_sw_ipl_02","h4_sw_ipl_02_lod","h4_sw_ipl_02_slod","h4_sw_ipl_03","h4_sw_ipl_03_lod","h4_sw_ipl_03_slod","h4_sw_ipl_04","h4_sw_ipl_04_lod","h4_sw_ipl_04_slod","h4_sw_ipl_05","h4_sw_ipl_05_lod","h4_sw_ipl_05_slod","h4_sw_ipl_06","h4_sw_ipl_06_lod","h4_sw_ipl_06_slod","h4_sw_ipl_07","h4_sw_ipl_07_lod","h4_sw_ipl_07_slod","h4_sw_ipl_08","h4_sw_ipl_08_lod","h4_sw_ipl_08_slod","h4_sw_ipl_09","h4_sw_ipl_09_lod","h4_sw_ipl_09_slod","h4_underwater_gate_closed","h4_islandx_placement_01","h4_islandx_placement_02","h4_islandx_placement_03","h4_islandx_placement_04","h4_islandx_placement_05","h4_islandx_placement_06","h4_islandx_placement_07","h4_islandx_placement_08","h4_islandx_placement_09","h4_islandx_placement_10","h4_mph4_island_placement"},
            {}, {}, true);

        ipl(LIT("North Yankton"), CMDNAMES("yank", "northyankton"),
            v3(3359.8732910156f, -4849.5229492188f, 111.67414855957f),
            {"plg_01","prologue01","prologue01c","prologue01d","prologue01e","prologue01f","prologue01g","prologue01h","prologue01i","prologue01j","prologue01k","prologue01z","plg_02","prologue02","plg_03","prologue03","prologue03_grv_cov","prologue03b","prologue_grv_torch","des_protree_end","des_protree_start","plg_04","prologue04","prologue04_cover","prologue04b","plg_05","prologue05","prologue05b","plg_06","prologue06","prologue06_int","prologue06_panne","prologue06b","prologue_m2_door","plg_rd","prologuerd","prologuerdb","prologue_DistantLights","prologue_LODLights"});

        createChild<CommandNorthYanktonMap>();

        ipl(LIT("Clucking Bell Factory"), CMDNAMES("cluckingbellfactory"),
            v3(-146.3837f, 6161.5f, 30.2062f),
            {"CS1_02_cf_onmission1","CS1_02_cf_onmission2","CS1_02_cf_onmission3","CS1_02_cf_onmission4"},
            {"CS1_02_cf_offmission"});

        ipl(LIT("USS Luxington"), CMDNAMES("ussluxington"),
            v3(3073.520264f, -4715.202148f, 16.089409f),
            {"hei_carrier","hei_carrier_DistantLights","hei_Carrier_int1","hei_Carrier_int2","hei_Carrier_int3","hei_Carrier_int4","hei_Carrier_int5","hei_Carrier_int6","hei_carrier_LODLights"},
            {}, {}, true);

        ipl(LIT("Morgue"), CMDNAMES("morgue"),
            v3(244.9f, -1374.7f, 39.5f),
            {"Coroner_Int_on","coronertrash"});

        ipl(LIT("Pillbox Hill Medical Center"), CMDNAMES("hospital"),
            v3(356.8f, -590.1f, 43.3f),
            {"RC12B_HospitalInterior"},
            {"RC12B_Default"},
            {"RC12B_Fixed","RC12B_Destroyed"});

        ipl(LIT("Series A Heist Yacht"), CMDNAMES("seriesayacht"),
            v3(-2045.8f, -1031.2f, 11.9f),
            {"hei_yacht_heist","hei_yacht_heist_Bar","hei_yacht_heist_Bedrm","hei_yacht_heist_Bridge","hei_yacht_heist_DistantLights","hei_yacht_heist_enginrm","hei_yacht_heist_LODLights","hei_yacht_heist_Lounge"},
            {}, {}, true);

        ipl(LIT("O'Neil Brothers Ranch"), CMDNAMES("ranch"),
            v3(2441.2f, 4968.5f, 51.7f),
            {"farm","farmint","farm_props","des_farmhouse"},
            {"farm_burnt","farm_burnt_props"},
            {"farmint_cap"});

        ipl(LIT("O'Neil Brothers Ranch Fire"), CMDNAMES("ranchfire"),
            v3(2447.9f, 4973.4f, 47.7f),
            {"des_farmhs_endimap","des_farmhs_end_occ","des_farmhs_startimap","des_farmhs_start_occ"});

        ipl(LIT("Lost MC Clubhouse"), CMDNAMES("lostmcclubhouse"),
            v3(984.1552f, -95.3662f, 74.50f),
            {"bkr_bi_hw1_13_int"},
            {"hei_bi_hw1_13_door"},
            {}, true);

        ipl(LIT("Lester's Factory"), CMDNAMES("lestersfactory"),
            v3(716.84f, -962.05f, 31.59f),
            {"id2_14_during_door","id2_14_during1"},
            {"id2_14_post_no_int"},
            {"id2_14_pre_no_int"});

        ipl(LIT("Fame or Shame"), CMDNAMES("fameorshame"),
            v3(-248.49159240722656f, -2010.509033203125f, 34.57429885864258f),
            {"sp1_10_real_interior"},
            {"sp1_10_fake_interior"});

        ipl(LIT("Lifeinvader"), CMDNAMES("lifeinvader"),
            v3(-1047.9f, -233.0f, 39.0f),
            {"facelobby"},
            {"facelobbyfake"});

        ipl(LIT("Vangelico Fine Jewelry"), CMDNAMES("vangelicofinejewelry"),
            v3(-630.4f, -236.7f, 40.0f),
            {"post_hiest_unload"},
            {"jewel2fake"},
            {"bh1_16_refurb"});

        ipl(LIT("Max Renda"), CMDNAMES("maxrenda"),
            v3(-585.8247f, -282.72f, 35.45475f),
            {"refit_unload"},
            {"bh1_16_doors_shut"});

        ipl(LIT("Train Crash"), CMDNAMES("trainwreck"),
            v3(-532.1309f, 4526.187f, 88.7955f),
            {"canyonriver01_traincrash","railing_end"},
            {"canyonriver01","railing_start"});

        ipl(LIT("Mount Chiliad UFO"), CMDNAMES("ufoeye", "eyeufo"),
            v3(496.58383f, 5608.7427f, 795.7164f),
            {"ufo_eye"});

        ipl(LIT("Banham Canyon House"), CMDNAMES("banhamcanyonhouse"),
            v3(-3096.308838f, 346.084503f, 10.804105f),
            {"ch1_02_open"});

        ipl(LIT("Red Carpet"), CMDNAMES("redcarpet"),
            v3(295.178833f, 177.504059f, 103.622993f),
            {"redCarpet"});

        ipl(LIT("FIB Lobby"), CMDNAMES("fiblobby"),
            v3(110.4f, -744.2f, 45.7f),
            {"FIBlobby"},
            {"FIBlobbyfake"});

        ipl(LIT("FIB Helicopter Crash"), CMDNAMES("fibhelicoptercrash"),
            v3(169.0f, -670.3f, 41.9f),
            {"DT1_05_HC_REQ"},
            {"DT1_05_HC_REMOVE"});

        ipl(LIT("FIB Rubble"), CMDNAMES("fibrubble"),
            v3(74.29f, -736.05f, 46.76f),
            {"DT1_05_rubble"});

        ipl(LIT("Union Depository"), CMDNAMES("uniondepository"),
            v3(2.69689322f, -667.0166f, 16.1306286f),
            {"FINBANK"});

        ipl(LIT("Plane Crash Trench"), CMDNAMES("planecrashtrench"),
            v3(2803.5776f, 4752.6777f, 46.337643f),
            {"Plane_crash_trench"});

        ipl(LIT("Alamo Sea Triathlon"), CMDNAMES("alamoseatriathlon"),
            v3(2384.969f, 4277.583f, 30.379f),
            {"CS2_06_TriAf02"});

        ipl(LIT("LSIA Triathlon"), CMDNAMES("lsiatriathlon"),
            v3(-1277.629f, -2030.913f, 1.2823f),
            {"AP1_04_TriAf01"});

        ipl(LIT("Fort Zancudo Gates"), CMDNAMES("zancudogates", "fortzancudogates"),
            v3(-1601.424f, 2808.213f, 16.2598f),
            {"cs3_07_mpgates"});

        ipl(LIT("Sandy Shores Airfield Boxes"), CMDNAMES("sandyshoresairfieldboxes"),
            v3(1743.682f, 3286.251f, 40.0875f),
            {"airfield"});

        ipl(LIT("Car Wash Spinners"), CMDNAMES("carwashspinners"),
            v3(55.7f, -1391.3f, 30.5f),
            {"Carwash_with_spinners"});

        ipl(LIT("Maze Bank Billboard Graffiti"), CMDNAMES("mazebankbillboardgraffiti"),
            v3(2697.32f, 3162.18f, 58.1f),
            {"CS5_04_MazeBillboardGraffiti"});

        ipl(LIT("Ron Oil Billboard Graffiti"), CMDNAMES("ronoilbillboardgraffiti"),
            v3(2101.490234f, 3071.507568f, 46.552769f),
            {"CS5_Roads_RonOilGraffiti"});

        ipl(LIT("iFruit Billboard"), CMDNAMES("ifruitbillboard"),
            v3(-1327.46f, -274.82f, 54.25f),
            {"FruitBB"});

        ipl(LIT("Meltdown Billboard 1"), CMDNAMES("meltdownbillboard1"),
            v3(-351.0f, -1324.0f, 44.02f),
            {"sc1_01_newbil"},
            {"SC1_01_OldBil"});

        ipl(LIT("Meltdown Billboard 2"), CMDNAMES("meltdownbillboard2"),
            v3(391.81f, -962.71f, 41.97f),
            {"dt1_17_newbil"},
            {"DT1_17_OldBil"});

        ipl(LIT("Hill Valley Church Grave"), CMDNAMES("hillvalleychurchgrave"),
            v3(-282.4638f, 2835.845f, 55.91446f),
            {"lr_cs6_08_grave_open"},
            {"lr_cs6_08_grave_closed"},
            {}, true);
    }
}
