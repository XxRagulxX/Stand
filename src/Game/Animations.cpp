#include "Game/Animations.hpp"

#include "Scripting/Natives.hpp"
#include "Scripting/Script.hpp"
#include "Util/Joaat.hpp"
#include "World/Self.hpp"

namespace Stand
{
	const std::vector<SimpleAnimation> Animations::scenarios = {
		{"Flex",                  "WORLD_HUMAN_MUSCLE_FLEX"},
		{"Smoke",                 "WORLD_HUMAN_SMOKING"},
		{"Sleep",                 "WORLD_HUMAN_BUM_SLUMPED"},
		{"Clap",                  "WORLD_HUMAN_CHEERING"},
		{"Guard",                 "WORLD_HUMAN_GUARD_STAND"},
		{"Hammer",                "WORLD_HUMAN_HAMMERING"},
		{"Idle Cop",              "WORLD_HUMAN_COP_IDLES"},
		{"Search Bin",            "PROP_HUMAN_BUM_BIN"},
		{"Shocked Filming",       "WORLD_HUMAN_MOBILE_FILM_SHOCKING"},
		{"Hangout",               "WORLD_HUMAN_HANG_OUT_STREET"},
		{"Impatient",             "WORLD_HUMAN_STAND_IMPATIENT"},
		{"Texting",               "WORLD_HUMAN_STAND_MOBILE"},
		{"Lean",                  "WORLD_HUMAN_LEANING"},
		{"Tend To Dead",          "CODE_HUMAN_MEDIC_TEND_TO_DEAD"},
		{"Wait",                  "CODE_HUMAN_CROSS_ROAD_WAIT"},
		{"Clean",                 "WORLD_HUMAN_MAID_CLEAN"},
		{"Use ATM",               "PROP_HUMAN_ATM"},
		{"Yoga",                  "WORLD_HUMAN_YOGA"},
		{"Statue",                "WORLD_HUMAN_HUMAN_STATUE"},
		{"Weld",                  "WORLD_HUMAN_WELDING"},
		{"Sunbathe Back",         "WORLD_HUMAN_SUNBATHE_BACK"},
		{"Sunbathe",              "WORLD_HUMAN_SUNBATHE"},
		{"High-Class Prostitute", "WORLD_HUMAN_PROSTITUTE_HIGH_CLASS"},
		{"Low-Class Prostitute",  "WORLD_HUMAN_PROSTITUTE_LOW_CLASS"},
		{"Notepad",               "CODE_HUMAN_MEDIC_TIME_OF_DEATH"},
		{"Kneel",                 "CODE_HUMAN_MEDIC_KNEEL"},
		{"Jog",                   "WORLD_HUMAN_JOG_STANDING"},
		{"Binoculars",            "WORLD_HUMAN_BINOCULARS"},
		{"Standing Bum",          "WORLD_HUMAN_BUM_STANDING"},
		{"Gardening",             "WORLD_HUMAN_GARDENER_PLANT"},
		{"Sit On Ground",         "WORLD_HUMAN_PICNIC"},
		{"Stand & Fish",          "WORLD_HUMAN_STAND_FISHING"},
		{"Browse",                "WORLD_HUMAN_WINDOW_SHOP_BROWSE"},
		{"Sit",                   "PROP_HUMAN_SEAT_BENCH"},
	};

	const std::vector<ComplexAnimation> Animations::dances = {
		{"Buttwiggle",     "anim@amb@nightclub@lazlow@hi_podium@",                    "danceidle_hi_11_buttwiggle_b_laz",   ForcedMovementType::FORCE_STILL},
		{"Clown Dance 1",  "move_clown@p_m_zero_idles@",                              "fidget_short_dance",                  ForcedMovementType::FORCE_STILL},
		{"Clown Dance 2",  "move_clown@p_m_two_idles@",                               "fidget_short_dance",                  ForcedMovementType::FORCE_STILL},
		{"Dance 1",        "anim@amb@nightclub@dancers@solomun_entourage@",            "mi_dance_facedj_17_v1_female^1",      ForcedMovementType::FORCE_STILL},
		{"Dance 2",        "anim@amb@nightclub@mini@dance@dance_solo@female@var_a@",   "high_center",                        ForcedMovementType::FORCE_STILL},
		{"Dance 3",        "anim@amb@nightclub@mini@dance@dance_solo@female@var_a@",   "high_center_up",                     ForcedMovementType::FORCE_STILL},
		{"Dance 4",        "anim@amb@nightclub@dancers@crowddance_facedj@hi_intensity","hi_dance_facedj_09_v2_female^1",     ForcedMovementType::FORCE_STILL},
		{"Dance 5",        "anim@amb@nightclub@dancers@crowddance_facedj@hi_intensity","hi_dance_facedj_09_v2_female^3",     ForcedMovementType::FORCE_STILL},
		{"Slow Dance 1",   "anim@amb@nightclub@mini@dance@dance_solo@female@var_a@",   "low_center",                         ForcedMovementType::FORCE_STILL},
		{"Slow Dance 2",   "anim@amb@nightclub@mini@dance@dance_solo@female@var_a@",   "low_center_down",                    ForcedMovementType::FORCE_STILL},
		{"Slow Dance 3",   "anim@amb@nightclub@mini@dance@dance_solo@female@var_b@",   "low_center",                         ForcedMovementType::FORCE_STILL},
		{"Podium Dance",   "anim@amb@nightclub@dancers@podium_dancers@",               "hi_dance_facedj_17_v2_male^5",       ForcedMovementType::FORCE_STILL},
		{"Solo Dance",     "anim@amb@nightclub@mini@dance@dance_solo@male@var_b@",     "high_center_down",                   ForcedMovementType::FORCE_STILL},
		{"Shy Dance 1",    "anim@amb@nightclub@mini@dance@dance_solo@male@var_a@",     "low_center",                         ForcedMovementType::FORCE_STILL},
		{"Shy Dance 2",    "anim@amb@nightclub@mini@dance@dance_solo@female@var_b@",   "low_center_down",                    ForcedMovementType::FORCE_STILL},
		{"Silly Dance 1",  "rcmnigel1bnmt_1b",                                         "dance_loop_tyler",                   ForcedMovementType::FORCE_STILL},
		{"Silly Dance 2",  "timetable@tracy@ig_5@idle_a",                              "idle_a",                             ForcedMovementType::FORCE_STILL},
		{"Silly Dance 3",  "timetable@tracy@ig_8@idle_b",                              "idle_d",                             ForcedMovementType::FORCE_STILL},
		{"Tao Dance",      "misschinese2_crystalmazemcs1_cs",                          "dance_loop_tao",                     ForcedMovementType::FORCE_STILL},
		{"The Woogie",     "anim@mp_player_intcelebrationfemale@the_woogie",           "the_woogie",                         ForcedMovementType::FORCE_STILL},
		{"Shuffle 1",      "anim@amb@nightclub@mini@dance@dance_solo@shuffle@",        "high_center",                        ForcedMovementType::FORCE_STILL},
		{"Shuffle 1 (Low)","anim@amb@nightclub@mini@dance@dance_solo@shuffle@",        "high_left_down",                     ForcedMovementType::FORCE_STILL},
		{"Shuffle 2",      "anim@amb@nightclub@mini@dance@dance_solo@shuffle@",        "high_right_up",                      ForcedMovementType::FORCE_STILL},
		{"Shuffle 2 (Low)","anim@amb@nightclub@mini@dance@dance_solo@shuffle@",        "high_right_down",                    ForcedMovementType::FORCE_STILL},
	};

	const std::vector<ComplexAnimation> Animations::animations = {
		{"Drink",                "mp_player_inteat@pnq",                                     "loop"},
		{"Beast",                "anim@mp_fm_event@intro",                                    "beast_transform"},
		{"Chill",                "switch@trevor@scares_tramp",                                "trev_scares_tramp_idle_tramp"},
		{"Cloudgazer",           "switch@trevor@annoys_sunbathers",                           "trev_annoys_sunbathers_loop_girl"},
		{"Prone",                "missfbi3_sniping",                                          "prone_dave"},
		{"Pull Over",            "misscarsteal3pullover",                                     "pull_over_right"},
		{"Wait",                 "amb@world_human_hang_out_street@female_hold_arm@idle_a",    "idle_a"},
		{"Drunk 1",              "random@drunk_driver_1",                                     "drunk_driver_stand_loop_dd1"},
		{"Drunk 2",              "random@drunk_driver_1",                                     "drunk_driver_stand_loop_dd2"},
		{"Drunk 3",              "missarmenian2",                                             "standing_idle_loop_drunk"},
		{"Air Guitar",           "anim@mp_player_intcelebrationfemale@air_guitar",            "air_guitar"},
		{"Air Synth",            "anim@mp_player_intcelebrationfemale@air_synth",             "air_synth"},
		{"Argue 1",              "misscarsteal4@actor",                                       "actor_berating_loop"},
		{"Argue 2",              "oddjobs@assassinate@vice@hooker",                           "argue_a"},
		{"Bartender",            "anim@amb@clubhouse@bar@drink@idle_a",                       "idle_a_bartender"},
		{"Blow Kiss",            "anim@mp_player_intcelebrationfemale@blow_kiss",             "blow_kiss"},
		{"Sarcastic",            "anim@mp_player_intcelebrationpaired@f_f_sarcastic",         "sarcastic_left"},
		{"Bring It On",          "misscommon@response",                                       "bring_it_on"},
		{"Come At Me",           "mini@triathlon",                                            "want_some_of_this"},
		{"Use Radio",            "amb@code_human_police_investigate@idle_a",                  "idle_b"},
		{"Arms Crossed 1",       "anim@amb@nightclub@peds@",                                  "rcmme_amanda1_stand_loop_cop"},
		{"Arms Crossed 2",       "amb@world_human_hang_out_street@female_arms_crossed@idle_a","idle_a"},
		{"Arms Crossed 3",       "anim@amb@nightclub@peds@",                                  "rcmme_amanda1_stand_loop_cop"},
		{"Damn",                 "gestures@m@standing@casual",                               "gesture_damn"},
		{"Stealing",             "anim@am_hold_up@male",                                     "shoplift_mid"},
		{"Facepalm 1",           "anim@mp_player_intcelebrationfemale@face_palm",            "face_palm"},
		{"Facepalm 2",           "random@car_thief@agitated@idle_a",                         "agitated_idle_a"},
		{"Fall Over",            "random@drunk_driver_1",                                     "drunk_fall_over"},
		{"Fall Asleep",          "mp_sleep",                                                  "sleep_loop"},
		{"Inspect",              "random@train_tracks",                                       "idle_e"},
		{"Jumping Jacks",        "timetable@reunited@ig_2",                                   "jimmy_getknocked"},
		{"Kneel",                "rcmextreme3",                                               "idle"},
		{"Knock",                "timetable@jimmy@doorknock@",                                "knockdoor_idle"},
		{"Lap Dance 1",          "mp_safehouse",                                              "lap_dance_girl"},
		{"Lap Dance 2",          "mini@strip_club@private_dance@idle",                        "priv_dance_idle"},
		{"Lap Dance 3",          "mini@strip_club@private_dance@part2",                       "priv_dance_p2"},
		{"Lap Dance 4",          "mini@strip_club@private_dance@part3",                       "priv_dance_p3"},
		{"Twerk",                "switch@trevor@mocks_lapdance",                              "001443_01_trvs_28_idle_stripper"},
		{"Lean",                 "amb@world_human_leaning@female@wall@back@hand_up@idle_a",   "idle_a"},
		{"Flirty Lean",          "random@street_race",                                        "_car_a_flirt_girl"},
		{"Mechanic",             "mini@repair",                                               "fixing_a_ped"},
		{"Meditate",             "rcmcollect_paperleadinout@",                                "meditiate_idle"},
		{"Out Of Breath",        "re@construction",                                           "out_of_breath"},
		{"Push Up",              "amb@world_human_push_ups@male@idle_a",                      "idle_d"},
		{"Race Start Countdown", "random@street_race",                                        "grid_girl_race_start"},
		{"Shrug",                "gestures@f@standing@casual",                               "gesture_shrug_hard"},
		{"Yoga Pose",            "missfam5_yoga",                                             "a2_pose"},
		{"Laugh",                "anim@arena@celeb@flat@paired@no_props@",                    "laugh_a_player_b"},
		{"Gang Sign",            "mp_player_int_uppergang_sign_a",                            "mp_player_int_gang_sign_a"},
		{"Faint",                "missarmenian2",                                             "drunk_loop"},
		{"Crawl",                "move_injured_ground",                                       "front_loop"},
		{"Flip 1",               "anim@arena@celeb@flat@solo@no_props@",                      "cap_a_player_a"},
		{"Flip 2",               "anim@arena@celeb@flat@solo@no_props@",                      "flip_a_player_a"},
		{"Slide 1",              "anim@arena@celeb@flat@solo@no_props@",                      "slide_a_player_a"},
		{"Slide 2",              "anim@arena@celeb@flat@solo@no_props@",                      "slide_b_player_a"},
		{"CPR",                  "mini@cpr@char_a@cpr_str",                                   "cpr_pumpchest"},
		{"Stretch",              "mini@triathlon",                                             "idle_e"},
		{"Celebrate",            "rcmfanatic1celebrate",                                      "celebrate"},
		{"Clown",                "rcm_barry2",                                                "clown_idle_0"},
		{"Nervous 1",            "amb@world_human_bum_standing@twitchy@idle_a",               "idle_c"},
		{"Nervous 2",            "rcmme_tracey1",                                             "nervous_loop"},
		{"Namaste",              "timetable@amanda@ig_4",                                     "ig_4_base"},
		{"DJ",                   "anim@amb@nightclub@djs@dixon@",                              "dixn_dance_cntr_open_dix"},
		{"Hands Up",             "missminuteman_1ig_2",                                       "handsup_base"},
		{"Surrender",            "random@arrests@busted",                                     "idle_a"},
		{"Whistle",              "rcmnigel1c",                                                "hailing_whistle_waive_a"},
		{"Lean, Arms Crossed",   "anim@amb@business@bgen@bgen_no_work@",                      "stand_phone_phoneputdown_idle_nowork"},
		{"Type On Keyboard",     "mp_prison_break",                                           "hack_loop"},
		{"Stand By Fire",        "amb@world_human_stand_fire@male@idle_a",                    "idle_a"},
		{"Thumbs Up",            "random@hitch_lift",                                         "idle_f"},
		{"Bow",                  "anim@arena@celeb@podium@no_prop@",                          "regal_c_1st"},
		{"Superhero",            "rcmbarry",                                                   "base"},
		{"Bend Over",            "anim@amb@carmeet@checkout_car@",                            "female_c_idle_d"},
		{"Shower",               "mp_safehouseshower@male@",                                  "male_shower_idle_b"},
		{"Clean Face",           "switch@michael@wash_face",                                  "loop_michael"},
		{"Karate",               "anim@mp_player_intcelebrationfemale@karate_chops",          "karate_chops"},
		{"Boxing",               "anim@mp_player_intcelebrationmale@shadow_boxing",           "shadow_boxing"},
	};

	const std::vector<ComplexObjectAnimation> Animations::object_animations = {
		{{"Briefcase",              "move_weapon@jerrycan@generic",                              "idle",                  ForcedMovementType::NONE},
		 {"bkr_prop_biker_case_shut"_J,  PedBoneId::PH_R_Hand,       0.1000f,  0.0100f,  0.0040f,    0.0f,    0.0f,  -90.0f}},
		{{"Drink Beer",             "amb@world_human_drinking@beer@male@idle_a",                 "idle_c",                ForcedMovementType::NONE},
		 {"prop_amb_beer_bottle"_J,      PedBoneId::PH_R_Hand,       0.0000f,  0.0000f,  0.0600f,    0.0f,   15.0f,    0.0f}},
		{{"Drink From Can",         "amb@world_human_drinking@coffee@male@idle_a",               "idle_c",                ForcedMovementType::NONE},
		 {"v_res_tt_can01"_J,            PedBoneId::PH_R_Hand,       0.0000f,  0.0000f,  0.0000f,    0.0f,    0.0f,   80.0f}},
		{{"Panhandle",              "amb@world_human_bum_freeway@male@base",                     "base",                  ForcedMovementType::NONE},
		 {"prop_beggers_sign_03"_J,      PedBoneId::SKEL_R_Finger20, 0.1900f,  0.1800f,  0.0000f,    5.0f,    0.0f,   40.0f}},
		{{"Play Bongos",            "amb@world_human_musician@bongos@male@base",                 "base",                  ForcedMovementType::NONE},
		 {"prop_bongos_01"_J,            PedBoneId::PH_L_Hand,       0.0000f,  0.0000f,  0.0000f,    0.0f,    0.0f,    0.0f}},
		{{"Read Book",              "cellphone@",                                                "cellphone_text_read_base", ForcedMovementType::NONE},
		 {"prop_novel_01"_J,             PedBoneId::IK_R_Hand,       0.1500f,  0.0300f, -0.0650f,    0.0f,  180.0f,   90.0f}},
		{{"Give Flowers",           "impexp_int-0",                                              "mp_m_waremech_01_dual-0", ForcedMovementType::NONE},
		 {"prop_snow_flower_02"_J,       PedBoneId::SKEL_Spine2,    -0.2900f,  0.4000f, -0.0200f,  -90.0f,  -90.0f,    0.0f}},
		{{"Sweep With Broom",       "anim@amb@drug_field_workers@rake@male_b@base",              "base",                  ForcedMovementType::FORCE_STILL},
		 {"prop_tool_broom"_J,           PedBoneId::PH_R_Hand,      -0.0100f,  0.0400f, -0.0300f,    0.0f,    0.0f,    0.0f}},
		{{"Carry Box of Beer",      "anim@heists@box_carry@",                                    "idle",                  ForcedMovementType::NONE},
		 {"prop_beer_box_01"_J,          PedBoneId::PH_R_Hand,       0.0200f, -0.0600f, -0.1200f, -180.0f, -180.0f,    1.99f}},
		{{"Carry Pizza",            "anim@heists@box_carry@",                                    "idle",                  ForcedMovementType::NONE},
		 {"prop_pizza_box_02"_J,         PedBoneId::PH_R_Hand,       0.0100f, -0.1000f, -0.1590f,   20.0f,    0.0f,    0.0f}},
		{{"Hold Champagne",         "anim@heists@humane_labs@finale@keycards",                   "ped_a_enter_loop",      ForcedMovementType::NONE},
		 {"prop_drink_champ"_J,          PedBoneId::SKEL_L_Hand,     0.1000f, -0.0300f,  0.0300f, -100.0f,    0.0f,  -10.0f}},
		{{"Drink Coffee",           "amb@world_human_drinking@coffee@male@idle_a",               "idle_c",                ForcedMovementType::NONE},
		 {"p_amb_coffeecup_01"_J,        PedBoneId::PH_R_Hand,       0.0000f,  0.0000f,  0.0000f,    0.0f,    0.0f,    0.0f}},
		{{"Tiny Duffel Bag",        "move_weapon@jerrycan@generic",                              "idle",                  ForcedMovementType::NONE},
		 {"bkr_prop_duffel_bag_01a"_J,   PedBoneId::PH_R_Hand,       0.2600f,  0.0400f,  0.0000f,   90.0f,    0.0f,  -78.99f}},
		{{"Hand Bag",               "move_weapon@jerrycan@generic",                              "idle",                  ForcedMovementType::NONE},
		 {"prop_amb_handbag_01"_J,       PedBoneId::PH_R_Hand,       0.3300f,  0.0500f, -0.0200f,   90.4294f,-177.4267f, 83.0011f}},
		{{"Play Guitar",            "amb@world_human_musician@guitar@male@idle_a",               "idle_b",                ForcedMovementType::NONE},
		 {"prop_acc_guitar_01"_J,        PedBoneId::SKEL_Spine3,    -0.1000f,  0.3100f,  0.1000f,    0.0f,   20.0f,  150.0f}},
		{{"Poor Mr. Raspberry",     "timetable@trevor@skull_loving_bear",                        "skull_loving_bear",     ForcedMovementType::NONE},
		 {"prop_mr_raspberry_01"_J,      PedBoneId::PH_R_Hand,       0.0000f,  0.0000f,  0.0000f,    0.0f,    0.0f,    0.0f}},
		{{"Sit in Lawnchair (F)",   "timetable@reunited@ig_10",                                  "base_amanda",           ForcedMovementType::FORCE_STILL},
		 {"prop_skid_chair_02"_J,        PedBoneId::SKEL_ROOT,       0.0250f,  0.0000f, -0.1500f,   45.0f,    5.0f,  180.0f}},
		{{"Sit in Lawnchair (M)",   "timetable@ron@ig_3_couch",                                  "base",                  ForcedMovementType::FORCE_STILL},
		 {"prop_skid_chair_02"_J,        PedBoneId::SKEL_ROOT,      -0.0500f,  0.0000f, -0.2000f,    5.0f,    0.0f,  180.0f}},
		{{"Examine Map",            "amb@code_human_in_bus_passenger_idles@female@tablet@idle_a","idle_a",                ForcedMovementType::FORCE_MOVEMENT},
		 {"prop_tourist_map_01"_J,       PedBoneId::PH_R_Hand,      -0.0500f,  0.0000f,  0.0000f,    0.0f,    0.0f,    0.0f}},
		{{"Hold Camera",            "missfinale_c2mcs_1",                                        "fin_c2_mcs_1_camman",   ForcedMovementType::NONE},
		 {"prop_v_cam_01"_J,             PedBoneId::PH_R_Hand,       0.0000f,  0.0300f,  0.0100f,    0.0f,    0.0f,    0.0f}},
		{{"Hold Microphone",        "anim@heists@humane_labs@finale@keycards",                   "ped_a_enter_loop",      ForcedMovementType::NONE},
		 {"p_ing_microphonel_01"_J,      PedBoneId::SKEL_L_Finger42, 0.0000f, -0.0200f,  0.1100f,    0.0f,    0.0f,   60.0f}},
		{{"Walk With Cane",         "missbigscore2aleadinout@bs_2a_2b_int",                      "lester_base_idle",      ForcedMovementType::NONE},
		 {"prop_cs_walking_stick"_J,     PedBoneId::PH_R_Hand,       0.0000f,  0.0000f,  0.0000f,    0.0f,    0.0f,    0.0f}},
		{{"Cellphone 1",            "cellphone@",                                                "cellphone_call_listen_base", ForcedMovementType::NONE},
		 {"prop_phone_proto"_J,          PedBoneId::PH_R_Hand,       0.0000f,  0.0000f,  0.0000f,    0.0f,    0.0f,    0.0f}},
		{{"Cellphone 2",            "random@kidnap_girl",                                        "ig_1_girl_on_phone_loop", ForcedMovementType::NONE},
		 {"prop_phone_proto"_J,          PedBoneId::PH_R_Hand,       0.0000f,  0.0000f,  0.0000f,    0.0f,    0.0f,    0.0f}},
		{{"Hold Sign",              "rcmnigel1d",                                                "base_club_shoulder",    ForcedMovementType::NONE},
		 {"prop_cs_protest_sign_01"_J,   PedBoneId::SKEL_R_Hand,     0.1820f,  0.2400f,  0.0600f,  -69.3774f,  5.9142f, -13.9572f}},
		{{"Hold Rose",              "anim@heists@humane_labs@finale@keycards",                   "ped_a_enter_loop",      ForcedMovementType::NONE},
		 {"prop_single_rose"_J,          PedBoneId::SKEL_L_Hand,     0.1300f,  0.1500f,  0.0000f, -100.0f,    0.0f,  -20.0f}},
		{{"Hold Freeway Sign",      "rcmnigel1d",                                                "base_club_shoulder",    ForcedMovementType::NONE},
		 {"prop_sign_freewayentrance"_J, PedBoneId::PH_L_Hand,      -0.1390f, -0.9870f,  0.4300f,  -67.3315f, 145.0628f,  -4.4319f}},
		{{"Hold \"No Parking\" Sign","rcmnigel1d",                                               "base_club_shoulder",    ForcedMovementType::NONE},
		 {"prop_sign_road_04a"_J,        PedBoneId::PH_L_Hand,      -0.1390f, -0.9870f,  0.4300f,  -67.3315f, 145.0628f,  -4.4319f}},
		{{"Hold Umbrella",          "rcmnigel1d",                                                "base_club_shoulder",    ForcedMovementType::NONE},
		 {"p_amb_brolly_01_s"_J,         PedBoneId::PH_R_Hand,       0.0700f,  0.0100f,  0.1100f,    2.3402f,-150.9606f,  57.3375f}},
		{{"Hold Whiskey Glass",     "amb@world_human_drinking@coffee@male@idle_a",               "idle_c",                ForcedMovementType::NONE},
		 {"prop_drink_whisky"_J,         PedBoneId::PH_R_Hand,       0.0100f, -0.0100f, -0.0600f,    0.0f,    0.0f,    0.0f}},
		{{"Hold Whiskey Bottle",    "amb@world_human_drinking@beer@male@idle_a",                 "idle_a",                ForcedMovementType::NONE},
		 {"ba_prop_battle_whiskey_bottle_2_s"_J, PedBoneId::PH_R_Hand, 0.0000f, 0.0000f, 0.0500f, 0.0f, 0.0f, 0.0f}},
		{{"Hold Wine Glass",        "anim@heists@humane_labs@finale@keycards",                   "ped_a_enter_loop",      ForcedMovementType::NONE},
		 {"prop_drink_redwine"_J,        PedBoneId::SKEL_L_Hand,     0.1000f, -0.0300f,  0.0300f, -100.0f,    0.0f,  -10.0f}},
		{{"Hold Wine Bottle",       "amb@world_human_drinking@beer@female@idle_a",               "idle_e",                ForcedMovementType::NONE},
		 {"prop_wine_rose"_J,            PedBoneId::PH_R_Hand,       0.0000f,  0.0400f, -0.1900f,   10.0f,    0.0f,    0.0f}},
		{{"Dance With Wine",        "anim@amb@nightclub_island@dancers@crowddance_single_props@","hi_dance_prop_09_v1_female^3", ForcedMovementType::FORCE_STILL},
		 {"p_wine_glass_s"_J,            PedBoneId::PH_R_Hand,       0.0000f,  0.0000f, -0.0900f,    0.0f,    0.0f,    0.0f}},
		{{"Dance With Beer",        "anim@amb@nightclub_island@dancers@beachdanceprop@",         "mi_idle_c_m01",         ForcedMovementType::FORCE_STILL},
		 {"prop_beer_amopen"_J,          PedBoneId::PH_R_Hand,       0.0000f,  0.0000f,  0.0000f,    0.0f,    0.0f,   20.0f}},
	};

	void Animations::loadDict(const char* dict)
	{
		STREAMING::REQUEST_ANIM_DICT(dict);
		while (!STREAMING::HAS_ANIM_DICT_LOADED(dict))
			Script::current()->yield();
	}

	void Animations::unloadDict(const char* dict)
	{
		STREAMING::REMOVE_ANIM_DICT(dict);
	}

	void Animations::reset()
	{
		const auto ped = Self::GetPed().GetHandle();
		if (TASK::PED_HAS_USE_SCENARIO_TASK(ped))
			TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);
		if (last_complex_animation != nullptr)
		{
			TASK::STOP_ANIM_TASK(ped,
				last_complex_animation->anim_dict,
				last_complex_animation->anim_name,
				1.0f);
			last_complex_animation = nullptr;
		}
		if (last_object_attachment != 0)
		{
			Object obj = last_object_attachment;
			OBJECT::DELETE_OBJECT(&obj);
			last_object_attachment = 0;
		}
	}
}
