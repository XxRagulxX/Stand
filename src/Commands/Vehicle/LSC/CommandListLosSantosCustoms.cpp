#include "Commands/Vehicle/LSC/CommandListLosSantosCustoms.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Label.hpp"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

namespace Stand
{
    namespace
    {
        static std::atomic<bool> s_lsc_in_veh{false};

        #define C(r,g,b) ((uint32_t)(r) | ((uint32_t)(g) << 8) | ((uint32_t)(b) << 16))
        static constexpr uint32_t kVehColourPalette[161] = {
            C(  0,  0,  0), C( 35, 33, 28), C( 40, 40, 40), C( 75, 75, 75), C(194,194,190), C(188,193,196), C(140,135,128), C(110,110,108),
            C(165,163,149), C(120,120,115), C( 97,104,109), C( 13, 17, 22), C(  6,  6,  6), C( 78, 78, 78), C(193,194,195), C( 19, 23, 20),
            C( 17, 30, 44), C( 83, 89, 83), C(165,168,163), C( 55, 55, 57), C( 83, 84, 87), C( 25, 26, 23), C( 57, 51, 44), C(121,115,106),
            C(175,172,167), C(170,172,173), C(140,135,128), C(188, 24,  8), C(206,  0,  2), C(239,  0,  0), C(232, 42, 11), C(185, 40, 26),
            C(151, 28, 14), C(213, 56, 26), C(100, 28,  0), C(255,  0,  0), C(255,119,  0), C(177,148,  0), C(255,130,  0), C(210,  0,  0),
            C(144,  0,  0), C(222,100,  0), C(243,215,  0), C(202, 10,  0), C(255, 25,  0), C(161, 28,  0), C(190, 30,  0), C(198, 55,  0),
            C(135, 10,  0), C(  0, 50,  0), C(  0, 89,  3), C(  0,111, 58), C( 70, 78, 10), C(  0,222,  0), C(  0,156, 74), C(155,196,  0),
            C(  0, 50,  0), C(  0,103,  0), C(  0, 54,  0), C(  0, 84,  0), C(  0, 87, 68), C( 35, 50,100), C(  0, 27, 87), C(104,127,167),
            C(  0,119,187), C(  0, 79,124), C(  0, 48, 68), C(135,161,190), C(  0,142,155), C(  0, 94,134), C(  9, 17,138), C( 82,  0,102),
            C(102,  0,152), C(  0, 86,163), C(156,196,211), C(  0, 45, 84), C(  0,  0, 50), C(  0, 79,158), C(  0, 93,133), C(  0,200,255),
            C(  0,164,224), C(  0,164,255), C(  0, 29, 56), C(  0, 80,175), C(  0,  0, 42), C(  0, 36, 64), C(  0, 53,107), C(127,170,185),
            C(255,233,  0), C(255,206,  0), C(100, 53,  0), C(155,181,  0), C(168,207,  0), C(205,192,145), C(106, 72, 36), C(106, 85, 64),
            C( 15,  9,  2), C(122, 81, 33), C(152,103, 56), C(185,169,123), C( 97,101, 40), C( 68, 50, 32), C(159,135, 92), C(147,112, 73),
            C(153,101, 55), C(162,128, 90), C(173,148,119), C(235,224,181), C(117, 84, 49), C(148,107, 60), C(179,137, 76), C(227,232,232),
            C(235,240,240), C(185,166,134), C(100, 72, 52), C( 51, 32, 17), C(190,175,129), C(114,118,122), C( 45, 47, 48), C(142,147,153),
            C(255,255,255), C(204,197,182), C(224,218,200), C(160, 88,  0), C(192,112,  0), C( 17, 38,  0), C(216,190,  0), C(  0,  0,128),
            C(  0,126,  0), C( 79, 52, 14), C(207,151, 93), C(225,230,230), C(215,218,210), C( 93, 96, 35), C(244,242,240), C(255,102,153),
            C(252,147,140), C(252,186,217), C(255,148,  0), C(  0,190,  0), C(  0,180,255), C(  0,  0, 45), C( 48,  0, 78), C( 86, 14,  8),
            C(  0, 65,  0), C(152,  0,255), C(  0,  0, 25), C( 14, 14, 14), C( 47,  0, 72), C( 27,  0, 47), C(148,  0,  0), C(  0, 48,  0),
            C( 57, 68,  0), C( 60, 43, 13), C(153,136, 81), C( 44, 78,  0), C(148,155,161), C(142,183,210), C(185,149,  0), C(158,139,  0),
            C(158,147,  0),
        };
        #undef C

        int LscGetVehicle()
        {
            int ped = PLAYER::GET_PLAYER_PED(-1);
            int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) return veh;
            veh = PED::GET_VEHICLE_PED_IS_IN(ped, true);
            return (veh && ENTITY::DOES_ENTITY_EXIST(veh)) ? veh : 0;
        }


        namespace Mod
        {
            constexpr int Spoilers    = 0;
            constexpr int FrontBumper = 1;
            constexpr int RearBumper  = 2;
            constexpr int SideSkirt   = 3;
            constexpr int Exhaust     = 4;
            constexpr int Frame       = 5;
            constexpr int Grille      = 6;
            constexpr int Hood        = 7;
            constexpr int Fender      = 8;
            constexpr int RFender     = 9;
            constexpr int Roof        = 10;
            constexpr int Engine      = 11;
            constexpr int Brakes      = 12;
            constexpr int Trans       = 13;
            constexpr int Horns       = 14;
            constexpr int Suspension  = 15;
            constexpr int Armour      = 16;
            constexpr int Turbo       = 18;
            constexpr int FrontWheels = 23;
            constexpr int PlateHolder = 25;
            constexpr int VanityPlates= 26;
            constexpr int Trim        = 27;
            constexpr int Ornaments   = 28;
            constexpr int Dashboard   = 29;
            constexpr int Dial        = 30;
            constexpr int DoorSpeaker = 31;
            constexpr int Seats       = 32;
            constexpr int Wheel       = 33;
            constexpr int Shifter     = 34;
            constexpr int Plaques     = 35;
            constexpr int Speakers    = 36;
            constexpr int Trunk       = 37;
            constexpr int Hydraulics  = 38;
            constexpr int EngineBlock = 39;
            constexpr int AirFilter   = 40;
            constexpr int Struts      = 41;
            constexpr int ArchCover   = 42;
            constexpr int Aerials     = 43;
            constexpr int Trim2       = 44;
            constexpr int Tank        = 45;
            constexpr int DoorL       = 46;
            constexpr int DoorR       = 47;
            constexpr int Livery      = 48;
            constexpr int Lightbar    = 49;

            static constexpr int visual[] = {
                Horns, Livery, SideSkirt, Suspension, Exhaust, Frame,
                FrontBumper, RearBumper, Hood, Roof, Fender, RFender,
                Grille, PlateHolder, VanityPlates, Trim, Ornaments,
                Dashboard, Dial, Seats, DoorSpeaker, Wheel, Shifter,
                Plaques, Speakers, Trunk, Hydraulics, EngineBlock,
                AirFilter, Struts, ArchCover, Aerials, Trim2, Tank,
                DoorL, DoorR, Lightbar,
            };

            const char* GetName(int t)
            {
                switch (t) {
                case Spoilers:    return "Spoilers";
                case FrontBumper: return "Front Bumper";
                case RearBumper:  return "Rear Bumper";
                case SideSkirt:   return "Side Skirt";
                case Exhaust:     return "Exhaust";
                case Frame:       return "Frame";
                case Grille:      return "Grille";
                case Hood:        return "Hood";
                case Fender:      return "Fender";
                case RFender:     return "Right Fender";
                case Roof:        return "Roof / Weapons";
                case Engine:      return "Engine";
                case Brakes:      return "Brakes";
                case Trans:       return "Transmission";
                case Horns:       return "Horns";
                case Suspension:  return "Suspension";
                case Armour:      return "Armour";
                case Turbo:       return "Turbo";
                case FrontWheels: return "Front Wheels";
                case PlateHolder: return "Plate Holders";
                case VanityPlates:return "Vanity Plates";
                case Trim:        return "Trim Design";
                case Ornaments:   return "Ornaments";
                case Dashboard:   return "Dashboard";
                case Dial:        return "Dial Design";
                case DoorSpeaker: return "Door Speaker";
                case Seats:       return "Seats";
                case Wheel:       return "Steering Wheel";
                case Shifter:     return "Shift Lever";
                case Plaques:     return "Plaques";
                case Speakers:    return "Speakers";
                case Trunk:       return "Trunk";
                case Hydraulics:  return "Hydraulics";
                case EngineBlock: return "Engine Block";
                case AirFilter:   return "Boost / Air Filter";
                case Struts:      return "Struts";
                case ArchCover:   return "Arch Cover";
                case Aerials:     return "Aerials";
                case Trim2:       return "Trim";
                case Tank:        return "Tank";
                case DoorL:       return "Left Door";
                case DoorR:       return "Right Door";
                case Livery:      return "Livery";
                case Lightbar:    return "Lightbar";
                default:          return "Mod";
                }
            }
        }

        constexpr const char* kStdColours[161] = {
            "Black", "Carbon Black", "Graphite", "Anthracite Black",
            "Black Steel", "Dark Steel", "Silver", "Bluish Silver",
            "Rolled Steel", "Shadow Silver", "Stone Silver",
            "Midnight Silver", "Cast Iron Silver",
            "Red", "Torino Red", "Formula Red", "Blaze Red",
            "Grace Red", "Garnet Red", "Sunset Red", "Cabernet Red",
            "Wine Red", "Candy Red",
            "Hot Pink", "Pfister Pink", "Salmon Pink",
            "Sunrise Orange", "Orange", "Bright Orange",
            "Gold", "Bronze",
            "Yellow", "Race Yellow", "Dew Yellow",
            "Dark Green", "Racing Green", "Sea Green", "Olive Green",
            "Bright Green", "Gasoline Green", "Lime Green",
            "Midnight Blue", "Galaxy Blue", "Dark Blue", "Saxon Blue",
            "Blue", "Mariner Blue", "Harbor Blue", "Diamond Blue",
            "Surf Blue", "Nautical Blue", "Racing Blue", "Ultra Blue",
            "Light Blue",
            "Chocolate Brown", "Bison Brown", "Creek Brown",
            "Feltzer Brown", "Maple Brown", "Beechwood Brown",
            "Sienna Brown", "Saddle Brown", "Moss Brown",
            "Woodbeech Brown", "Straw Brown", "Sandy Brown",
            "Bleached Brown",
            "Schafter Purple", "Spinnaker Purple", "Midnight Purple",
            "Bright Purple",
            "Cream", "Ice White", "Frost White", "Pure White", "White",
            "Yogurt", "Rose",
            "Gun Metal",
            "Colour 79", "Colour 80", "Colour 81", "Colour 82",
            "Colour 83", "Colour 84", "Colour 85", "Colour 86",
            "Colour 87", "Colour 88", "Colour 89", "Colour 90",
            "Colour 91", "Colour 92", "Colour 93", "Colour 94",
            "Colour 95", "Colour 96", "Colour 97", "Colour 98",
            "Colour 99",  "Colour 100", "Colour 101", "Colour 102",
            "Colour 103", "Colour 104", "Colour 105", "Colour 106",
            "Colour 107", "Colour 108", "Colour 109", "Colour 110",
            "Colour 111", "Colour 112", "Colour 113", "Colour 114",
            "Colour 115", "Colour 116", "Colour 117", "Colour 118",
            "Colour 119", "Colour 120", "Colour 121", "Colour 122",
            "Colour 123", "Colour 124", "Colour 125", "Colour 126",
            "Colour 127", "Colour 128", "Colour 129", "Colour 130",
            "Colour 131", "Colour 132", "Colour 133", "Colour 134",
            "Colour 135", "Colour 136", "Colour 137", "Colour 138",
            "Colour 139", "Colour 140", "Colour 141", "Colour 142",
            "Colour 143", "Colour 144", "Colour 145", "Colour 146",
            "Colour 147", "Colour 148", "Colour 149", "Colour 150",
            "Colour 151", "Colour 152", "Colour 153", "Colour 154",
            "Colour 155", "Colour 156", "Colour 157", "Colour 158",
            "Colour 159", "Colour 160",
        };

        struct RGB { int r, g, b; };
        struct HSV { float h, s, v; };

        HSV RgbToHsv(RGB c)
        {
            float r = c.r / 255.f, g = c.g / 255.f, b = c.b / 255.f;
            float mx = std::max({r, g, b}), mn = std::min({r, g, b});
            float d  = mx - mn;
            float h  = 0.f;
            if (d > 0.f) {
                if      (mx == r) h = 60.f * std::fmod((g - b) / d, 6.f);
                else if (mx == g) h = 60.f * ((b - r) / d + 2.f);
                else              h = 60.f * ((r - g) / d + 4.f);
                if (h < 0.f) h += 360.f;
            }
            return { h, mx > 0.f ? d / mx : 0.f, mx };
        }

        RGB HsvToRgb(HSV c)
        {
            float x = c.s * c.v;
            float m = c.v - x;
            float k = c.h / 60.f;
            float f = k - std::floor(k);
            float p = m, q = m + x * (1.f - f), t = m + x * f, v = m + x;
            float r, g, b;
            switch (static_cast<int>(k) % 6) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            default:r = v; g = p; b = q; break;
            }
            return { int(r * 255.f + .5f), int(g * 255.f + .5f), int(b * 255.f + .5f) };
        }

        static RGB GetVehicleColourRgb(int veh, bool primary)
        {
            int r = 0, g = 0, b = 0;
            bool custom = primary
                ? VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh)
                : VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh);
            if (custom) {
                if (primary) VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &r, &g, &b);
                else         VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &r, &g, &b);
            } else {
                int pri, sec;
                VEHICLE::GET_VEHICLE_COLOURS(veh, &pri, &sec);
                int idx = primary ? pri : sec;
                uint32_t p = (idx >= 0 && idx <= 160) ? kVehColourPalette[idx] : 0u;
                r = int(p & 0xFF); g = int((p >> 8) & 0xFF); b = int((p >> 16) & 0xFF);
            }
            return {r, g, b};
        }

        class CommandVehmodInt : public CommandSlider
        {
            const int m_mod;
        public:
            explicit CommandVehmodInt(CommandList* parent, int modType)
                : CommandSlider(parent, LIT(Mod::GetName(modType)), CMDNAMES_0(), NOLABEL, -1, -1, -1)
                , m_mod(modType)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandVehmodInt() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                int veh = LscGetVehicle();
                s_lsc_in_veh.store(veh != 0, std::memory_order_relaxed);

                if (!veh) { setMaxValue(min_value); return; }
                int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, m_mod) - 1;
                setMaxValue(mx);
                if (mx >= min_value) {
                    Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                    setValueIndicator(c, VEHICLE::GET_VEHICLE_MOD(veh, m_mod));
                }
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                int v = value, mt = m_mod;
                click.ensureScriptThread([v, mt] {
                    int veh = LscGetVehicle();
                    if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    VEHICLE::SET_VEHICLE_MOD(veh, mt, v, false);
                });
            }
        };

        class CommandVehmodBool : public CommandToggle
        {
            const int m_mod;
        public:
            explicit CommandVehmodBool(CommandList* parent, int modType, std::vector<CommandName> cmds = {})
                : CommandToggle(parent, LIT(Mod::GetName(modType)), std::move(cmds))
                , m_mod(modType)
            {}

            void onChange(Click& click) override
            {
                if (click.isAuto()) return;
                bool on = m_on; int mt = m_mod;
                click.ensureScriptThread([on, mt] {
                    int veh = LscGetVehicle();
                    if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, mt, on);
                });
            }
        };

        class CommandVehfinish : public CommandSlider
        {
            std::function<void(int)> m_apply;
            static const char* FinishName(int v) {
                switch (v) {
                case 0: return "Normal";
                case 1: return "Metallic";
                case 2: return "Pearlescent";
                case 3: return "Matte";
                case 4: return "Metal";
                case 5: return "Chrome";
                case 6: return "Chameleon";
                default: return "";
                }
            }
        public:
            CommandVehfinish(CommandList* parent, std::vector<CommandName> cmds, std::function<void(int)> apply)
                : CommandSlider(parent, LIT("Finish"), std::move(cmds), NOLABEL, 0, 6, 0)
                , m_apply(std::move(apply)) {}

            std::string getValueText() const override { return FinishName(value); }

            void onChange(Click& click, int) override {
                int v = value; auto fn = m_apply;
                click.ensureScriptThread([v, fn] { fn(v); });
            }
        };

        enum class ColourTarget { Primary, Secondary, Pearlescent, Interior, Wheel };

        class CommandStdColourAction : public CommandPhysical
        {
            ColourTarget m_tgt;
            int          m_idx;
        public:
            CommandStdColourAction(CommandList* parent, Label name, ColourTarget tgt, int idx)
                : CommandPhysical(COMMAND_ACTION, parent, std::move(name), CMDNAMES_0(), NOLABEL)
                , m_tgt(tgt), m_idx(idx) {}

            void onClick(Click& click) override {
                int ci = m_idx; ColourTarget tgt = m_tgt;
                click.ensureScriptThread([ci, tgt] {
                    int veh = LscGetVehicle();
                    if (!veh) return;
                    int pe = 0, wh = 0;
                    VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    switch (tgt) {
                    case ColourTarget::Primary: {
                        VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh);
                        int p, s; VEHICLE::GET_VEHICLE_COLOURS(veh, &p, &s);
                        VEHICLE::SET_VEHICLE_COLOURS(veh, ci, s);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                        break;
                    }
                    case ColourTarget::Secondary: {
                        VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh);
                        int p, s; VEHICLE::GET_VEHICLE_COLOURS(veh, &p, &s);
                        VEHICLE::SET_VEHICLE_COLOURS(veh, p, ci);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                        break;
                    }
                    case ColourTarget::Pearlescent:
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, ci, wh);
                        break;
                    case ColourTarget::Interior:
                        VEHICLE::SET_VEHICLE_EXTRA_COLOUR_5(veh, ci);
                        break;
                    case ColourTarget::Wheel:
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, ci);
                        break;
                    }
                });
            }
        };

        void AddStdColours(CommandList* parent, ColourTarget tgt)
        {
            for (int i = 0; i < 161; ++i)
                parent->createChild<CommandStdColourAction>(LIT(kStdColours[i]), tgt, i);
        }

        class CommandColourChannelSlider : public CommandSlider
        {
            std::function<void(int)> m_apply;
            std::function<int()>     m_read;
        public:
            CommandColourChannelSlider(CommandList* parent, Label name,
                                       std::function<void(int)> apply,
                                       std::function<int()> read = nullptr)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, 255, 0)
                , m_apply(std::move(apply))
                , m_read(std::move(read))
            { if (m_read) CommandTickDispatch::AddCommand(this); }
            ~CommandColourChannelSlider() override { if (m_read) CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int v = m_read();
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, v);
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value; auto fn = m_apply;
                click.ensureScriptThread([v, fn] { fn(v); });
            }
        };

        class CommandVehRainbow : public CommandSlider
        {
            std::function<void(RGB)> m_setter;
            time_t                   m_lastTick = 0;
            float                    m_hue      = 0.f;
            bool                     m_ticking  = false;
        public:
            CommandVehRainbow(CommandList* parent, std::vector<CommandName> cmds, std::function<void(RGB)> setter)
                : CommandSlider(parent, LIT("Rainbow Mode (0 to 1000)"), std::move(cmds),
                      LIT("Cycles colour hue every x milliseconds. 0 = off."), 0, 1000, 0)
                , m_setter(std::move(setter)) {}

            ~CommandVehRainbow() override {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            void onChange(Click& click, int) override {
                if (value > 0 && !m_ticking) {
                    CommandTickDispatch::AddCommand(this);
                    m_ticking = true;
                } else if (value == 0 && m_ticking) {
                    CommandTickDispatch::RemoveCommand(this);
                    m_ticking = false;
                }
            }

            void onTick() override {
                if (value <= 0) return;
                auto now = get_current_time_millis();
                if (now - m_lastTick < static_cast<time_t>(value)) return;
                m_lastTick = now;
                m_hue = std::fmod(m_hue + 1.f, 360.f);
                m_setter(HsvToRgb({ m_hue, 1.f, 1.f }));
            }
        };

        class CommandCopyPrimaryToSecondary : public CommandPhysical
        {
        public:
            explicit CommandCopyPrimaryToSecondary(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Primary Colour"), CMDNAMES_0(), NOLABEL) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle();
                    if (!veh) return;
                    int r, g, b;
                    VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &r, &g, &b);
                    int pe, wh;
                    VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, r, g, b);
                    VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                });
            }
        };

        class CommandColourSectionHeader : public CommandPhysical
        {
        public:
            CommandColourSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class CommandHsvChannelPrimary : public CommandSlider
        {
            int m_ch;
        public:
            CommandHsvChannelPrimary(CommandList* parent, Label name, int max_val, int ch)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_ch(ch)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandHsvChannelPrimary() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle(); if (!veh) return;
                HSV hsv = RgbToHsv(GetVehicleColourRgb(veh, true));
                int newVal;
                if      (m_ch == 0) newVal = int(std::round(hsv.h));
                else if (m_ch == 1) newVal = int(std::round(hsv.s * 100.f));
                else                newVal = int(std::round(hsv.v * 100.f));
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, newVal);
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value; int ch = m_ch;
                click.ensureScriptThread([v, ch] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    HSV hsv = RgbToHsv(GetVehicleColourRgb(veh, true));
                    if      (ch == 0) hsv.h = float(v);
                    else if (ch == 1) hsv.s = v / 100.f;
                    else              hsv.v = v / 100.f;
                    RGB rgb = HsvToRgb(hsv);
                    int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                    VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                });
            }
        };

        class CommandCurrentPrimaryHex : public CommandSlider
        {
            std::atomic<uint32_t> m_packed{0};
        public:
            explicit CommandCurrentPrimaryHex(CommandList* parent)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandCurrentPrimaryHex() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                uint32_t p = 0;
                if (veh) {
                    RGB rgb = GetVehicleColourRgb(veh, true);
                    p = (uint32_t(rgb.r) << 16) | (uint32_t(rgb.g) << 8) | uint32_t(rgb.b);
                    p |= 0x80000000u;
                }
                m_packed.store(p, std::memory_order_relaxed);
            }

            std::string getValueText() const override {
                uint32_t p = m_packed.load(std::memory_order_relaxed);
                if (!(p & 0x80000000u)) return "--------";
                char buf[9];
                snprintf(buf, sizeof(buf), "%06X00", p & 0x00FFFFFFu);
                return buf;
            }
        };

        class CommandPrimaryNavBarSync : public CommandPhysical
        {
        public:
            explicit CommandPrimaryNavBarSync(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), {}, NOLABEL, CMDFLAG_CONCEALED)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandPrimaryNavBarSync() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                uint32_t packed = 0u;
                auto* myGrid = &Rendering::GridStandCommandList::GetOrCreate(this->parent);
                if (Rendering::MenuNavigation::Current() == myGrid) {
                    int veh = LscGetVehicle();
                    if (veh) {
                        RGB rgb = GetVehicleColourRgb(veh, true);
                        packed = 0x01000000u | uint32_t(rgb.r) | (uint32_t(rgb.g) << 8) | (uint32_t(rgb.b) << 16);
                    }
                }
                Rendering::Theme::kNavBarColour.store(packed, std::memory_order_relaxed);
            }
        };

        class CommandVehcolourPrimary : public CommandList
        {
        public:
            explicit CommandVehcolourPrimary(CommandList* parent)
                : CommandList(parent, LIT("Primary Colour"), CMDNAMES("vehprimary", "vehicleprimary"))
            {
                createChild<CommandPrimaryNavBarSync>();
                createChild<CommandVehRainbow>(CMDNAMES("vehprimaryrainbow"), [](RGB rgb) {
                    int veh = LscGetVehicle(); if (!veh) return;
                    int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                    VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                });
                createChild<CommandVehfinish>(CMDNAMES("vehprimaryfinish"), [](int finish) {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    if (finish == 5) {
                        int p, s; VEHICLE::GET_VEHICLE_COLOURS(veh, &p, &s);
                        VEHICLE::SET_VEHICLE_COLOURS(veh, 120, s);
                    } else {
                        VEHICLE::SET_VEHICLE_MOD_COLOR_1(veh, finish, 0, 0);
                    }
                    VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                });
                auto* lsc = createChild<CommandList>(LIT("LSC Colours"), CMDNAMES("vehprimarylsc"));
                AddStdColours(lsc, ColourTarget::Primary);

                createChild<CommandColourSectionHeader>("HSV Representation");
                createChild<CommandHsvChannelPrimary>(LIT("Hue"),        360, 0);
                createChild<CommandHsvChannelPrimary>(LIT("Saturation"), 100, 1);
                createChild<CommandHsvChannelPrimary>(LIT("Value"),      100, 2);

                createChild<CommandColourSectionHeader>("RGB Representation");
                createChild<CommandColourChannelSlider>(LIT("Red"),
                    [](int v) {
                        int veh = LscGetVehicle(); if (!veh) return;
                        RGB cur = GetVehicleColourRgb(veh, true);
                        int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                        VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, v, cur.g, cur.b);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                    },
                    []() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, true).r; }
                );
                createChild<CommandColourChannelSlider>(LIT("Green"),
                    [](int v) {
                        int veh = LscGetVehicle(); if (!veh) return;
                        RGB cur = GetVehicleColourRgb(veh, true);
                        int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                        VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, cur.r, v, cur.b);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                    },
                    []() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, true).g; }
                );
                createChild<CommandColourChannelSlider>(LIT("Blue"),
                    [](int v) {
                        int veh = LscGetVehicle(); if (!veh) return;
                        RGB cur = GetVehicleColourRgb(veh, true);
                        int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                        VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, cur.r, cur.g, v);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                    },
                    []() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, true).b; }
                );

                createChild<CommandColourSectionHeader>("Other");
                createChild<CommandCurrentPrimaryHex>();
            }
        };

        class CommandHsvChannelSecondary : public CommandSlider
        {
            int m_ch;
        public:
            CommandHsvChannelSecondary(CommandList* parent, Label name, int max_val, int ch)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_ch(ch)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandHsvChannelSecondary() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle(); if (!veh) return;
                HSV hsv = RgbToHsv(GetVehicleColourRgb(veh, false));
                int newVal;
                if      (m_ch == 0) newVal = int(std::round(hsv.h));
                else if (m_ch == 1) newVal = int(std::round(hsv.s * 100.f));
                else                newVal = int(std::round(hsv.v * 100.f));
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, newVal);
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value; int ch = m_ch;
                click.ensureScriptThread([v, ch] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    HSV hsv = RgbToHsv(GetVehicleColourRgb(veh, false));
                    if      (ch == 0) hsv.h = float(v);
                    else if (ch == 1) hsv.s = v / 100.f;
                    else              hsv.v = v / 100.f;
                    RGB rgb = HsvToRgb(hsv);
                    int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                    VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                });
            }
        };

        class CommandCurrentSecondaryHex : public CommandSlider
        {
            std::atomic<uint32_t> m_packed{0};
        public:
            explicit CommandCurrentSecondaryHex(CommandList* parent)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandCurrentSecondaryHex() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                uint32_t p = 0;
                if (veh) {
                    RGB rgb = GetVehicleColourRgb(veh, false);
                    p = (uint32_t(rgb.r) << 16) | (uint32_t(rgb.g) << 8) | uint32_t(rgb.b);
                    p |= 0x80000000u;
                }
                m_packed.store(p, std::memory_order_relaxed);
            }

            std::string getValueText() const override {
                uint32_t p = m_packed.load(std::memory_order_relaxed);
                if (!(p & 0x80000000u)) return "--------";
                char buf[9];
                snprintf(buf, sizeof(buf), "%06X00", p & 0x00FFFFFFu);
                return buf;
            }
        };

        class CommandVehcolourSecondary : public CommandList
        {
        public:
            explicit CommandVehcolourSecondary(CommandList* parent)
                : CommandList(parent, LIT("Secondary Colour"), CMDNAMES("vehsecondary", "vehiclesecondary"))
            {
                createChild<CommandVehRainbow>(CMDNAMES("vehsecondaryrainbow"), [](RGB rgb) {
                    int veh = LscGetVehicle(); if (!veh) return;
                    int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                    VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                });
                createChild<CommandVehfinish>(CMDNAMES("vehsecondaryfinish"), [](int finish) {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    if (finish == 5) {
                        int p, s; VEHICLE::GET_VEHICLE_COLOURS(veh, &p, &s);
                        VEHICLE::SET_VEHICLE_COLOURS(veh, p, 120);
                    } else {
                        VEHICLE::SET_VEHICLE_MOD_COLOR_2(veh, finish, 0);
                    }
                    VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                });
                auto* lsc = createChild<CommandList>(LIT("LSC Colours"), CMDNAMES("vehsecondarylsc"));
                AddStdColours(lsc, ColourTarget::Secondary);
                createChild<CommandCopyPrimaryToSecondary>();

                createChild<CommandColourSectionHeader>("HSV Representation");
                createChild<CommandHsvChannelSecondary>(LIT("Hue"),        360, 0);
                createChild<CommandHsvChannelSecondary>(LIT("Saturation"), 100, 1);
                createChild<CommandHsvChannelSecondary>(LIT("Value"),      100, 2);

                createChild<CommandColourSectionHeader>("RGB Representation");
                createChild<CommandColourChannelSlider>(LIT("Red"),
                    [](int v) {
                        int veh = LscGetVehicle(); if (!veh) return;
                        RGB cur = GetVehicleColourRgb(veh, false);
                        int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                        VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, v, cur.g, cur.b);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                    },
                    []() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, false).r; }
                );
                createChild<CommandColourChannelSlider>(LIT("Green"),
                    [](int v) {
                        int veh = LscGetVehicle(); if (!veh) return;
                        RGB cur = GetVehicleColourRgb(veh, false);
                        int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                        VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, cur.r, v, cur.b);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                    },
                    []() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, false).g; }
                );
                createChild<CommandColourChannelSlider>(LIT("Blue"),
                    [](int v) {
                        int veh = LscGetVehicle(); if (!veh) return;
                        RGB cur = GetVehicleColourRgb(veh, false);
                        int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                        VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, cur.r, cur.g, v);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                    },
                    []() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, false).b; }
                );

                createChild<CommandColourSectionHeader>("Other");
                createChild<CommandCurrentSecondaryHex>();
            }
        };

        class CommandNeonLight : public CommandToggle
        {
            const int m_idx;
        public:
            CommandNeonLight(CommandList* parent, Label name, std::vector<CommandName> cmds, int idx)
                : CommandToggle(parent, std::move(name), std::move(cmds))
                , m_idx(idx) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on; int i = m_idx;
                click.ensureScriptThread([on, i] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_NEON_ENABLED(veh, i, on);
                });
            }
        };

        class CommandNeonAll : public CommandToggle
        {
        public:
            explicit CommandNeonAll(CommandList* parent)
                : CommandToggle(parent, LIT("All"), CMDNAMES("vehneonall")) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on;
                click.ensureScriptThread([on] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    for (int i = 0; i < 4; ++i)
                        VEHICLE::SET_VEHICLE_NEON_ENABLED(veh, i, on);
                });
            }
        };

        class CommandWheelType : public CommandSlider
        {
            static const char* WheelTypeName(int v) {
                switch (v) {
                case 0:  return "Sport";
                case 1:  return "Muscle";
                case 2:  return "Lowrider";
                case 3:  return "SUV";
                case 4:  return "Offroad";
                case 5:  return "Tuner";
                case 6:  return "Bike";
                case 7:  return "High End";
                case 8:  return "Benny's";
                case 9:  return "Benny's Bespoke";
                case 10: return "Race";
                case 11: return "Street";
                case 12: return "Track";
                default: return "";
                }
            }
        public:
            explicit CommandWheelType(CommandList* parent)
                : CommandSlider(parent, LIT("Wheel Type"), CMDNAMES("wheeltype"), NOLABEL, 0, 12, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandWheelType() override { CommandTickDispatch::RemoveCommand(this); }

            std::string getValueText() const override { return WheelTypeName(value); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, v);
                    VEHICLE::SET_VEHICLE_MOD(veh, Mod::FrontWheels, 0, false);
                });
            }
        };

        class CommandVehDriftTyres : public CommandToggle
        {
        public:
            explicit CommandVehDriftTyres(CommandList* parent)
                : CommandToggle(parent, LIT("Drift Tyres"), CMDNAMES("drifttyres")) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on;
                click.ensureScriptThread([on] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_DRIFT_TYRES(veh, on);
                });
            }
        };

        class CommandVehBulletproofTyres : public CommandToggle
        {
        public:
            explicit CommandVehBulletproofTyres(CommandList* parent)
                : CommandToggle(parent, LIT("Bulletproof Tyres"), CMDNAMES("bulletprooftyres")) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on;
                click.ensureScriptThread([on] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, !on);
                });
            }
        };

        class CommandIndependenceTires : public CommandToggle
        {
        public:
            explicit CommandIndependenceTires(CommandList* parent)
                : CommandToggle(parent, LIT("Independence Day Tyre Smoke"), CMDNAMES("independencetiresmoke")) {}

            void onChange(Click& click) override {
                if (!m_on) return;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 20, true);
                    VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, 0, 0, 0);
                    return true;
                });
            }
        };

        class CommandPlateType : public CommandSlider
        {
            static const char* PlateTypeName(int v) {
                switch (v) {
                case 0:  return "Blue-on-White 1";
                case 1:  return "Blue-on-White 2";
                case 2:  return "Blue-on-White 3";
                case 3:  return "Yellow-on-Black";
                case 4:  return "Yellow-on-Blue";
                case 5:  return "North Yankton";
                default: return "";
                }
            }
        public:
            explicit CommandPlateType(CommandList* parent)
                : CommandSlider(parent, LIT("Plate Type"), CMDNAMES("platetype"), NOLABEL, 0, 5, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandPlateType() override { CommandTickDispatch::RemoveCommand(this); }

            std::string getValueText() const override { return PlateTypeName(value); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, v);
                });
            }
        };

        class CommandPlateRandomise : public CommandPhysical
        {
        public:
            explicit CommandPlateRandomise(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Randomise Plate"), CMDNAMES("platerandomise"), NOLABEL) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    static const char kChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
                    char plate[9] = {};
                    for (int i = 0; i < 8; ++i)
                        plate[i] = kChars[rand() % (sizeof(kChars) - 1)];
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plate);
                });
            }
        };

        class CommandScrollingPlateText : public CommandToggle
        {
            std::string m_baseText;
            int         m_scrollPos = 0;
            time_t      m_lastTick  = 0;
        public:
            explicit CommandScrollingPlateText(CommandList* parent)
                : CommandToggle(parent, LIT("Scrolling Plate Text"), CMDNAMES("platescroll", "scrollplatetext")) {}

            void onChange(Click& click) override {
                if (!m_on) return;
                m_baseText.clear();
                m_scrollPos = 0;
                m_lastTick  = 0;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    if (m_baseText.empty()) {
                        const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                        m_baseText = (p && *p) ? p : "PLATE";
                    }
                    auto now = get_current_time_millis();
                    if (now - m_lastTick < 200) return true;
                    m_lastTick = now;
                    std::string padded = m_baseText + "        ";
                    int len = static_cast<int>(padded.size());
                    int pos = m_scrollPos % len;
                    std::string display = padded.substr(pos, 8);
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, display.c_str());
                    ++m_scrollPos;
                    return true;
                });
            }
        };

        class CommandPlateSpeed : public CommandToggle
        {
            std::string m_savedPlate;
            time_t      m_lastTick = 0;
        public:
            explicit CommandPlateSpeed(CommandList* parent)
                : CommandToggle(parent, LIT("Speed on Plate"), CMDNAMES("platespeedometer", "platespeed")) {}

            void onChange(Click& click) override {
                if (!m_on) {
                    if (!m_savedPlate.empty()) {
                        std::string plate = m_savedPlate;
                        click.ensureScriptThread([plate] {
                            int veh = LscGetVehicle(); if (!veh) return;
                            VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plate.c_str());
                        });
                        m_savedPlate.clear();
                    }
                    return;
                }
                m_lastTick = 0;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    if (m_savedPlate.empty()) {
                        const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                        m_savedPlate = (p && *p) ? p : "";
                    }
                    auto now = get_current_time_millis();
                    if (now - m_lastTick < 500) return true;
                    m_lastTick = now;
                    float spd = ENTITY::GET_ENTITY_SPEED(veh) * 3.6f;
                    char buf[16];
                    std::snprintf(buf, sizeof(buf), "%.0f KMH", spd);
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, buf);
                    return true;
                });
            }
        };

        class CommandPlateTps : public CommandToggle
        {
            std::string m_savedPlate;
            time_t      m_lastTick  = 0;
            int         m_frames    = 0;
            time_t      m_fpsTimer  = 0;
            int         m_fps       = 0;
        public:
            explicit CommandPlateTps(CommandList* parent)
                : CommandToggle(parent, LIT("FPS on Plate"), CMDNAMES("platetps")) {}

            void onChange(Click& click) override {
                if (!m_on) {
                    if (!m_savedPlate.empty()) {
                        std::string plate = m_savedPlate;
                        click.ensureScriptThread([plate] {
                            int veh = LscGetVehicle(); if (!veh) return;
                            VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plate.c_str());
                        });
                        m_savedPlate.clear();
                    }
                    return;
                }
                m_lastTick = m_fpsTimer = get_current_time_millis();
                m_frames   = 0;
                m_fps      = 0;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    if (m_savedPlate.empty()) {
                        const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                        m_savedPlate = (p && *p) ? p : "";
                    }
                    auto now = get_current_time_millis();
                    ++m_frames;
                    if (now - m_fpsTimer >= 1000) {
                        m_fps     = m_frames;
                        m_frames  = 0;
                        m_fpsTimer = now;
                    }
                    if (now - m_lastTick < 500) return true;
                    m_lastTick = now;
                    char buf[16];
                    std::snprintf(buf, sizeof(buf), "%d FPS", m_fps);
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, buf);
                    return true;
                });
            }
        };

        class CommandLockPlate : public CommandToggle
        {
            std::string m_lockedPlate;
        public:
            explicit CommandLockPlate(CommandList* parent)
                : CommandToggle(parent, LIT("Lock Plate"), CMDNAMES("lockplate")) {}

            void onChange(Click& click) override {
                if (!m_on) { m_lockedPlate.clear(); return; }
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    if (m_lockedPlate.empty()) {
                        const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                        m_lockedPlate = (p && *p) ? p : "LOCKED";
                    }
                    VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, m_lockedPlate.c_str());
                    return true;
                });
            }
        };

        class CommandHeadlights : public CommandSlider
        {
        public:
            explicit CommandHeadlights(CommandList* parent)
                : CommandSlider(parent, LIT("Headlights Colour"), CMDNAMES("headlights"), NOLABEL, 0, 12, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandHeadlights() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_XENON_LIGHT_COLOR_INDEX(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(veh, v);
                });
            }
        };

        class CommandWindowTint : public CommandSlider
        {
            static const char* TintName(int v) {
                switch (v) {
                case 0: return "None";
                case 1: return "Pure Black";
                case 2: return "Dark Smoke";
                case 3: return "Light Smoke";
                case 4: return "Stock";
                case 5: return "Limo";
                case 6: return "Green";
                default: return "";
                }
            }
        public:
            explicit CommandWindowTint(CommandList* parent)
                : CommandSlider(parent, LIT("Window Tint"), CMDNAMES("windowtint"), NOLABEL, 0, 6, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandWindowTint() override { CommandTickDispatch::RemoveCommand(this); }

            std::string getValueText() const override { return TintName(value); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_WINDOW_TINT(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, v);
                });
            }
        };

        class CommandVehRoofLivery : public CommandSlider
        {
        public:
            explicit CommandVehRoofLivery(CommandList* parent)
                : CommandSlider(parent, LIT("Roof Livery"), CMDNAMES("rooflivery"), NOLABEL, -1, -1, -1)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandVehRoofLivery() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) { setMaxValue(min_value); return; }
                int cnt = VEHICLE::GET_VEHICLE_LIVERY2_COUNT(veh);
                if (cnt <= 0) { setMaxValue(min_value); return; }
                setMaxValue(cnt - 1);
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                setValueIndicator(c, VEHICLE::GET_VEHICLE_LIVERY2(veh));
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_LIVERY2(veh, v);
                });
            }
        };

        class CommandScorched : public CommandToggle
        {
        public:
            explicit CommandScorched(CommandList* parent)
                : CommandToggle(parent, LIT("Scorched"), CMDNAMES("scorched", "bbq")) {}

            void onChange(Click& click) override {
                if (!m_on) {
                    click.ensureScriptThread([] {
                        int veh = LscGetVehicle(); if (!veh) return;
                        VEHICLE::SET_VEHICLE_ENVEFF_SCALE(veh, 1.f);
                    });
                    return;
                }
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = LscGetVehicle();
                    if (!veh) return true;
                    VEHICLE::SET_VEHICLE_ENVEFF_SCALE(veh, 0.f);
                    return true;
                });
            }
        };

        class CommandTonk : public CommandToggle
        {
        public:
            explicit CommandTonk(CommandList* parent)
                : CommandToggle(parent, LIT("Remove Turret"), CMDNAMES("tonk", "removeturret")) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on;
                click.ensureScriptThread([on] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_TURRET_HIDDEN(veh, 0, on);
                    VEHICLE::SET_TURRET_HIDDEN(veh, 1, on);
                });
            }
        };

        class CommandVehicleExtra : public CommandToggle
        {
            const int m_extra;
            static constexpr const char* kExtraNames[15] = {
                "Extra 1",  "Extra 2",  "Extra 3",  "Extra 4",  "Extra 5",
                "Extra 6",  "Extra 7",  "Extra 8",  "Extra 9",  "Extra 10",
                "Extra 11", "Extra 12", "Extra 13", "Extra 14", "Extra 15",
            };
        public:
            CommandVehicleExtra(CommandList* parent, int extra)
                : CommandToggle(parent, LIT(kExtraNames[extra]), CMDNAMES_0())
                , m_extra(extra) {}

            void onChange(Click& click) override {
                if (click.isAuto()) return;
                bool on = m_on; int ex = m_extra;
                click.ensureScriptThread([on, ex] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    if (!VEHICLE::DOES_EXTRA_EXIST(veh, ex)) return;
                    VEHICLE::SET_VEHICLE_EXTRA(veh, ex, !on);
                });
            }
        };

        class CommandListLscPerformance : public CommandList
        {
        public:
            explicit CommandListLscPerformance(CommandList* parent)
                : CommandList(parent, LIT("Performance"), CMDNAMES_0())
            {
                createChild<CommandVehmodInt>(Mod::Armour);
                createChild<CommandVehmodInt>(Mod::Brakes);
                createChild<CommandVehmodInt>(Mod::Engine);
                createChild<CommandVehmodInt>(Mod::Spoilers);
                createChild<CommandVehmodInt>(Mod::Trans);
                createChild<CommandVehmodBool>(Mod::Turbo, CMDNAMES("turbo"));
            }

            bool requiresVehicle() const override { return true; }
            const char* vehicleRequiredMessage() const override
            {
                return s_lsc_in_veh.load(std::memory_order_relaxed) ? nullptr : "Get your ass in a vehicle :/";
            }
        };

        class CommandLscPresetTunings : public CommandSlider
        {
        public:
            explicit CommandLscPresetTunings(CommandList* parent)
                : CommandSlider(parent, LIT(""), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1) {}

            std::string getValueText() const override { return "Preset Tunings"; }
        };

        class CommandTune : public CommandPhysical
        {
        public:
            explicit CommandTune(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Upgrade"), CMDNAMES("tune", "upgrade", "fullupgrade"), LIT("Fully upgrades your current or last vehicle.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    for (int i = 0; i < 50; ++i) {
                        if (i == 17) continue;
                        int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1;
                        if (mx >= 0) VEHICLE::SET_VEHICLE_MOD(veh, i, mx, false);
                    }
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, true);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 20, true);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, true);
                });
            }
        };

        class CommandPerf : public CommandPhysical
        {
        public:
            explicit CommandPerf(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Performance Upgrade"), CMDNAMES("performance", "perf"), LIT("Upgrades your current or last vehicle's EMS, Brakes, Transmission, Armour and Turbo.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    constexpr int perfMods[] = {
                        Mod::Engine, Mod::Brakes, Mod::Trans, Mod::Suspension, Mod::Armour
                    };
                    for (int t : perfMods) {
                        int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, t) - 1;
                        if (mx >= 0) VEHICLE::SET_VEHICLE_MOD(veh, t, mx, false);
                    }
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, Mod::Turbo, true);
                });
            }
        };

        class CommandPerfWithSpoiler : public CommandPhysical
        {
        public:
            explicit CommandPerfWithSpoiler(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Performance Upgrade, Including Spoiler"), CMDNAMES("perfwithspoiler"), LIT("Upgrades your current or last vehicle's EMS, Brakes, Transmission, Armour, Turbo and best Spoiler.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    constexpr int perfMods[] = {
                        Mod::Spoilers, Mod::Engine, Mod::Brakes,
                        Mod::Trans, Mod::Suspension, Mod::Armour
                    };
                    for (int t : perfMods) {
                        int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, t) - 1;
                        if (mx >= 0) VEHICLE::SET_VEHICLE_MOD(veh, t, mx, false);
                    }
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, Mod::Turbo, true);
                });
            }
        };

        class CommandRandomTune : public CommandPhysical
        {
        public:
            explicit CommandRandomTune(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Random Upgrade"), CMDNAMES("randomtune"), LIT("Applies random mods to your current or last vehicle.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    for (int i = 0; i < 50; ++i) {
                        if (i == 17) continue;
                        int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1;
                        if (mx > 0)
                            VEHICLE::SET_VEHICLE_MOD(veh, i, rand() % (mx + 1), false);
                    }
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, rand() % 2 != 0);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 20, rand() % 2 != 0);
                    VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, rand() % 2 != 0);
                });
            }
        };

        class CommandUntune : public CommandPhysical
        {
        public:
            explicit CommandUntune(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Downgrade"), CMDNAMES("untune", "downgrade"), LIT("Resets your current or last vehicle to stock tuning.")) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                    for (int i = 0; i < 50; ++i) {
                        VEHICLE::SET_VEHICLE_MOD(veh, i, -1, false);
                        VEHICLE::TOGGLE_VEHICLE_MOD(veh, i, false);
                    }
                });
            }
        };

    }

    CommandListLosSantosCustoms::CommandListLosSantosCustoms(CommandList* parent)
        : CommandList(parent, LIT("Los Santos Customs"), CMDNAMES("lsc", "customs", "lossantoscustoms"))
    {
        createChild<CommandListLscPerformance>();

        auto* appear = createChild<CommandList>(LIT("Appearance"), CMDNAMES_0());

        appear->createChild<CommandVehcolourPrimary>();
        appear->createChild<CommandVehcolourSecondary>();

        auto* pearl = appear->createChild<CommandList>(LIT("Pearlescent Colour"), CMDNAMES("vehpearlescent"));
        AddStdColours(pearl, ColourTarget::Pearlescent);

        auto* neons = appear->createChild<CommandList>(LIT("Neon Lights"), CMDNAMES("neoncolour"));
        {
            auto* neonCol = neons->createChild<CommandList>(LIT("Neon Colour"), CMDNAMES_0());
            neonCol->createChild<CommandColourChannelSlider>(LIT("Red"), [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, v, g, b);
            });
            neonCol->createChild<CommandColourChannelSlider>(LIT("Green"), [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, r, v, b);
            });
            neonCol->createChild<CommandColourChannelSlider>(LIT("Blue"), [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, r, g, v);
            });
            neonCol->createChild<CommandVehRainbow>(CMDNAMES("neoncolourrainbow"), [](RGB rgb) {
                int veh = LscGetVehicle(); if (!veh) return;
                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, rgb.r, rgb.g, rgb.b);
            });
            neons->createChild<CommandNeonAll>();
            neons->createChild<CommandNeonLight>(LIT("Front"),  CMDNAMES("vehneonfront"), 2);
            neons->createChild<CommandNeonLight>(LIT("Back"),   CMDNAMES("vehneonback"),  3);
            neons->createChild<CommandNeonLight>(LIT("Left"),   CMDNAMES("vehneonleft"),  0);
            neons->createChild<CommandNeonLight>(LIT("Right"),  CMDNAMES("vehneonright"), 1);
        }

        auto* wheels = appear->createChild<CommandList>(LIT("Wheels"), CMDNAMES_0());
        {
            wheels->createChild<CommandWheelType>();
            wheels->createChild<CommandVehmodInt>(Mod::FrontWheels);

            auto* wheelCol = wheels->createChild<CommandList>(LIT("Wheel Colour"), CMDNAMES("vehwheelcolour"));
            AddStdColours(wheelCol, ColourTarget::Wheel);

            auto* smokeCol = wheels->createChild<CommandList>(LIT("Tyre Smoke Colour"), CMDNAMES("vehtire"));
            smokeCol->createChild<CommandColourChannelSlider>(LIT("Red"), [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, v, g, b);
            });
            smokeCol->createChild<CommandColourChannelSlider>(LIT("Green"), [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, r, v, b);
            });
            smokeCol->createChild<CommandColourChannelSlider>(LIT("Blue"), [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, r, g, v);
            });
            smokeCol->createChild<CommandVehRainbow>(CMDNAMES("vehtiresmokrainbow"), [](RGB rgb) {
                int veh = LscGetVehicle(); if (!veh) return;
                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rgb.r, rgb.g, rgb.b);
            });

            wheels->createChild<CommandIndependenceTires>();
            wheels->createChild<CommandVehDriftTyres>();
            wheels->createChild<CommandVehBulletproofTyres>();
        }

        auto* plate = appear->createChild<CommandList>(LIT("License Plate"), CMDNAMES_0());
        {
            plate->createChild<CommandPlateType>();
            plate->createChild<CommandPlateRandomise>();
            plate->createChild<CommandScrollingPlateText>();
            plate->createChild<CommandPlateSpeed>();
            plate->createChild<CommandPlateTps>();
            plate->createChild<CommandLockPlate>();
        }

        appear->createChild<CommandList>(LIT("Parachute"), CMDNAMES_0());

        auto* extras = appear->createChild<CommandList>(LIT("Extras"), CMDNAMES_0());
        for (int i = 0; i < 15; ++i)
            extras->createChild<CommandVehicleExtra>(i);

        appear->createChild<CommandScorched>();
        appear->createChild<CommandTonk>();
        appear->createChild<CommandToggle>(LIT("Show Non-Applicable"), CMDNAMES("lscshownonapplicable"), NOLABEL);

        auto* interior = appear->createChild<CommandList>(LIT("Interior Colour"), CMDNAMES("vehinteriorcolour"));
        AddStdColours(interior, ColourTarget::Interior);

        appear->createChild<CommandHeadlights>();
        appear->createChild<CommandVehmodBool>(22, CMDNAMES("xenonlights"));
        appear->createChild<CommandWindowTint>();
        appear->createChild<CommandVehRoofLivery>();

        for (int t : Mod::visual)
            appear->createChild<CommandVehmodInt>(t);

        createChild<CommandLscPresetTunings>();
        createChild<CommandTune>();
        createChild<CommandPerf>();
        createChild<CommandPerfWithSpoiler>();
        createChild<CommandRandomTune>();
        createChild<CommandUntune>();
    }

}
