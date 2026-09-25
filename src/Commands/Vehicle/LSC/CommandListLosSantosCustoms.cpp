#include "Commands/Vehicle/LSC/CommandListLosSantosCustoms.hpp"
#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Core/Pointers.hpp"
#include "Menu/Click.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Util/Label.hpp"

#include <atomic>
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
        static std::atomic<bool> s_lsc_show_na{false};

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
                Horns, SideSkirt, Suspension, Exhaust, 
                FrontBumper, RearBumper, Hood, Roof, Fender, RFender,
                Grille, PlateHolder, VanityPlates, Trim, Ornaments,
                Dashboard, Dial, Seats, DoorSpeaker, Wheel, Shifter,
                Plaques, Speakers, Trunk, Hydraulics, EngineBlock,
                AirFilter, Struts, ArchCover, Aerials, Trim2, Tank,
                DoorL, DoorR, Lightbar, Livery, Frame,
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

                if (!veh) { setMaxValue(min_value); flags |= CMDFLAG_CONCEALED; return; }
                int mx = VEHICLE::GET_NUM_VEHICLE_MODS(veh, m_mod) - 1;
                setMaxValue(mx);
                bool na = (mx < 0);
                if (na && !s_lsc_show_na.load(std::memory_order_relaxed))
                    flags |= CMDFLAG_CONCEALED;
                else
                    flags &= ~CMDFLAG_CONCEALED;
                if (!na) {
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

        class CommandListScrollingText : public CommandList
        {
        public:
            std::string m_scrollText;
            int m_intervalMs = 250;

            explicit CommandListScrollingText(CommandList* parent)
                : CommandList(parent, LIT("Scrolling Text"), CMDNAMES("platescroll", "scrollplatetext"))
            {
                struct CommandScrollingToggle : public CommandToggle {
                    int m_scrollPos = 0;
                    time_t m_lastTick = 0;
                    explicit CommandScrollingToggle(CommandList* p)
                        : CommandToggle(p, LIT("Scrolling Text"), CMDNAMES_0()) {}

                    void onChange(Click& click) override {
                        if (!m_on) return;
                        m_scrollPos = 0;
                        m_lastTick = 0;
                        onChangeToggleScriptTickEventHandler(click, [this] {
                            if (!m_on) return false;
                            int veh = LscGetVehicle(); if (!veh) return true;
                            auto* list = static_cast<CommandListScrollingText*>(this->parent);
                            std::string text = list->m_scrollText;
                            if (text.empty()) {
                                const char* p = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
                                text = (p && *p) ? p : "PLATE";
                            }
                            auto now = get_current_time_millis();
                            if (now - m_lastTick < (time_t)list->m_intervalMs) return true;
                            m_lastTick = now;
                            std::string padded = text + "        ";
                            int pos = m_scrollPos % (int)padded.size();
                            VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, padded.substr(pos, 8).c_str());
                            ++m_scrollPos;
                            return true;
                        });
                    }
                };
                createChild<CommandScrollingToggle>();

                struct CommandScrollingTextSetter : public CommandPhysical {
                    explicit CommandScrollingTextSetter(CommandList* p)
                        : CommandPhysical(COMMAND_ACTION, p, LIT("Text"), CMDNAMES_0(), NOLABEL, CMDFLAG_TEXT_INPUT) {}

                    void onClick(Click& click) override {
                        auto* list = static_cast<CommandListScrollingText*>(this->parent);
                        Rendering::MenuCommandBox::Open(
                            "platescrolltext",
                            "Scrolling Text",
                            "Text to scroll across the plate",
                            list->m_scrollText,
                            [list, this](const std::string& text) -> bool {
                                list->m_scrollText = text;
                                setMenuName(text.empty() ? LIT("Text") : Label("Text: " + text, Label::TagLiteral{}));
                                return true;
                            }
                        );
                    }
                };
                createChild<CommandScrollingTextSetter>();

                struct CommandScrollingInterval : public CommandSlider {
                    explicit CommandScrollingInterval(CommandList* p)
                        : CommandSlider(p, LIT("Interval (ms)"), CMDNAMES_0(), NOLABEL, 50, 2000, 250) {}

                    void onChange(Click& click, int) override {
                        if (click.isAuto()) return;
                        static_cast<CommandListScrollingText*>(this->parent)->m_intervalMs = value;
                    }
                };
                createChild<CommandScrollingInterval>();
            }
        };

        class CommandPlateSpeed : public CommandToggle
        {
            std::string m_savedPlate;
            time_t      m_lastTick = 0;
        public:
            explicit CommandPlateSpeed(CommandList* parent)
                : CommandToggle(parent, LIT("Speedometer"), CMDNAMES("platespeedometer", "platespeed")) {}

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
                : CommandToggle(parent, LIT("Ticks Per Second"), CMDNAMES("platetps")) {}

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
                : CommandToggle(parent, LIT("Lock License Plate Text"), CMDNAMES("lockplate")) {}

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
                : CommandSlider(parent, LIT("Headlights"), CMDNAMES("headlights"), NOLABEL, 0, 12, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandHeadlights() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                if (!veh) return;
                Click c(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                if (VEHICLE::IS_TOGGLE_MOD_ON(veh, 22)) {
                    int colour = VEHICLE::GET_VEHICLE_XENON_LIGHT_COLOR_INDEX(veh);
                    setValueIndicator(c, colour == 255 ? 1 : colour + 2);
                } else {
                    setValueIndicator(c, 0);
                }
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value;
                click.ensureScriptThread([v] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    if (v == 0) {
                        VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, false);
                    } else {
                        VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, true);
                        VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(veh, v == 1 ? 255 : v - 2);
                    }
                });
            }
        };

        class CommandWindowTint : public CommandSlider
        {
        public:
            explicit CommandWindowTint(CommandList* parent)
                : CommandSlider(parent, LIT("Window Tint"), CMDNAMES("windowtint"), NOLABEL, 0, 6, 0)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandWindowTint() override { CommandTickDispatch::RemoveCommand(this); }

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
        public:
            CommandVehicleExtra(CommandList* parent, int extra)
                : CommandToggle(parent,
                    Label(std::string("Extra ") + std::to_string(extra), Label::TagLiteral{}),
                    CMDNAMES_0())
                , m_extra(extra)
            {
                CommandTickDispatch::AddCommand(this);
            }

            ~CommandVehicleExtra() override
            {
                CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int veh = LscGetVehicle();
                if (!veh || !VEHICLE::DOES_EXTRA_EXIST(veh, m_extra))
                {
                    flags |= CMDFLAG_CONCEALED;
                    return;
                }
                flags &= ~CMDFLAG_CONCEALED;
                m_on = (VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(veh, m_extra) != 0);
            }

            void onChange(Click& click) override
            {
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

        class CommandLscPresetTunings : public CommandPhysical
        {
        public:
            explicit CommandLscPresetTunings(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Preset Tunings"), CMDNAMES_0(), NOLABEL, CMDFLAG_SECTION_HEADER) {}
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

        class CommandParachuteEnable : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandParachuteEnable(CommandList* parent)
                : CommandSlider(parent, LIT("Enable Parachute"), CMDNAMES("vehparachute"), NOLABEL, 0, 2, 0) {}

            ~CommandParachuteEnable() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "On";
                case 2: return "Off";
                default: return "Don't Override";
                }
            }

            void onTick() override
            {
                int veh = LscGetVehicle();
                if (!veh) return;
                void* cveh = Pointers.HandleToPtr(veh);
                if (!cveh) return;
                void* model = *reinterpret_cast<void**>(reinterpret_cast<char*>(cveh) + 0x20);
                if (!model) return;
                uint8_t& b = *reinterpret_cast<uint8_t*>(reinterpret_cast<char*>(model) + 0x57C + 16);
                if (value == 1)
                    b |= 0x01u;
                else
                    b &= static_cast<uint8_t>(~0x01u);
            }

            void onChange(Click& click, int) override
            {
                if (value == 0)
                {
                    if (m_ticking) { CommandTickDispatch::RemoveCommand(this); m_ticking = false; }
                }
                else
                {
                    if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
                }
            }
        };

        struct ParachuteVariant { const char* name; uint32_t hash; int tint; };
        static constexpr ParachuteVariant kParachuteVariants[] = {
            { "General: Camo",             "sr_prop_specraces_para_s_01"_J, 0 },
            { "General: Red",              "sr_prop_specraces_para_s_01"_J, 1 },
            { "General: White/Blue/Yellow","sr_prop_specraces_para_s_01"_J, 2 },
            { "General: Lilac/Red/White",  "sr_prop_specraces_para_s_01"_J, 3 },
            { "General: Red/White/Blue",   "sr_prop_specraces_para_s_01"_J, 4 },
            { "General: Blue",             "sr_prop_specraces_para_s_01"_J, 5 },
            { "General: Lilac",            "sr_prop_specraces_para_s_01"_J, 6 },
            { "General: Lilac/Yellow",     "sr_prop_specraces_para_s_01"_J, 7 },
            { "General: Yellow/Blue/White","prop_v_parachute"_J,            0 },
            { "General: Orange",           "gr_prop_gr_para_s_01"_J,        0 },
            { "General: White",            "p_parachute1_sp_dec"_J,         0 },
            { "SecuroServ",                "imp_prop_impexp_para_s"_J,      0 },
            { "Broken",                    "p_para_broken1_s"_J,            0 },
        };

        class CommandParachuteModel : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandParachuteModel(CommandList* parent)
                : CommandSlider(parent, LIT("Appearance"), CMDNAMES("vehparamodel"), NOLABEL, 0, 12, 11) {}

            ~CommandParachuteModel() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                return kParachuteVariants[value].name;
            }

            void onTick() override
            {
                int veh = LscGetVehicle();
                if (!veh) return;
                VEHICLE::VEHICLE_SET_PARACHUTE_MODEL_OVERRIDE(veh, kParachuteVariants[value].hash);
                VEHICLE::VEHICLE_SET_PARACHUTE_MODEL_TINT_INDEX(veh, kParachuteVariants[value].tint);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
            }
        };

        class CommandLscShowNonApplicable : public CommandToggle
        {
        public:
            explicit CommandLscShowNonApplicable(CommandList* parent)
                : CommandToggle(parent, LIT("Show Non-Applicable"), CMDNAMES("lscshownonapplicable")) {}

            void onChange(Click& click) override
            {
                s_lsc_show_na.store(m_on, std::memory_order_relaxed);
            }
        };

    }

    CommandListLosSantosCustoms::CommandListLosSantosCustoms(CommandList* parent)
        : CommandList(parent, LIT("Los Santos Customs"), CMDNAMES("lsc", "customs", "lossantoscustoms"))
    {
        createChild<CommandListLscPerformance>();

        auto* appear = createChild<CommandList>(LIT("Appearance"), CMDNAMES_0());

        appear->createChild<CommandVehicleColour>(LIT("Primary Colour"), CMDNAMES("vehprimary", "vehicleprimary"), ColourTarget::Primary);
        appear->createChild<CommandVehicleColour>(LIT("Secondary Colour"), CMDNAMES("vehsecondary", "vehiclesecondary"), ColourTarget::Secondary);

        auto* pearl = appear->createChild<CommandList>(LIT("Pearlescent Colour"), CMDNAMES("vehpearlescent"));
        AddStdColours(pearl, ColourTarget::Pearlescent, true);

        auto* neons = appear->createChild<CommandList>(LIT("Neon Lights"), CMDNAMES("neoncolour"));
        {
            neons->createChild<CommandNeonColour>();
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

            wheels->createChild<CommandTyreSmokeColour>();
            wheels->createChild<CommandVehDriftTyres>();
            wheels->createChild<CommandVehBulletproofTyres>();
        }

        auto* plate = appear->createChild<CommandList>(LIT("License Plate"), CMDNAMES_0());
        {
            plate->createChild<CommandPlateType>();
            plate->createChild<CommandPlateRandomise>();
            plate->createChild<CommandListScrollingText>();
            plate->createChild<CommandPlateSpeed>();
            plate->createChild<CommandPlateTps>();
            plate->createChild<CommandLockPlate>();
        }

        auto* interior = appear->createChild<CommandList>(LIT("Interior Colour"), CMDNAMES("vehinteriorcolour"));
        AddStdColours(interior, ColourTarget::Interior);

        {
            auto* para = appear->createChild<CommandList>(LIT("Parachute"), CMDNAMES_0());
            para->createChild<CommandParachuteEnable>();
            para->createChild<CommandParachuteModel>();
        }

        auto* extras = appear->createChild<CommandList>(LIT("Extras"), CMDNAMES_0());
        for (int i = 1; i <= 14; ++i)
            extras->createChild<CommandVehicleExtra>(i);

        appear->createChild<CommandScorched>();
        appear->createChild<CommandTonk>();
        appear->createChild<CommandLscShowNonApplicable>();

        appear->createChild<CommandHeadlights>();
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
