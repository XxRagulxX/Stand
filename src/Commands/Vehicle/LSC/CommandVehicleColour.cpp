#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"

#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandIssuable.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Util/get_current_time_millis.hpp"
#include "Vehicle/ModNames.hpp"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

namespace Stand
{
    namespace
    {
        class CommandColourSectionHeader : public CommandPhysical
        {
        public:
            CommandColourSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        RGB GetColourByTarget(int veh, ColourTarget tgt)
        {
            switch (tgt) {
            case ColourTarget::Primary:   return GetVehicleColourRgb(veh, true);
            case ColourTarget::Secondary: return GetVehicleColourRgb(veh, false);
            case ColourTarget::Neon:      return GetNeonColourRgb(veh);
            case ColourTarget::TyreSmoke: return GetTyreSmokeColourRgb(veh);
            default:                      return {0, 0, 0};
            }
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

        class CommandHsvChannel : public CommandSlider
        {
            int  m_ch;
            ColourTarget m_tgt;
        public:
            CommandHsvChannel(CommandList* parent, Label name, int max_val, int ch, ColourTarget tgt)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_ch(ch), m_tgt(tgt)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandHsvChannel() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle(); if (!veh) return;
                HSV hsv = RgbToHsv(GetColourByTarget(veh, m_tgt));
                int newVal;
                if      (m_ch == 0) newVal = int(std::round(hsv.h));
                else if (m_ch == 1) newVal = int(std::round(hsv.s * 100.f));
                else                newVal = int(std::round(hsv.v * 100.f));
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, newVal);
            }

            void onChange(Click& click, int) override {
                if (click.isAuto()) return;
                int v = value; int ch = m_ch; ColourTarget tgt = m_tgt;
                click.ensureScriptThread([v, ch, tgt] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    HSV hsv = RgbToHsv(GetColourByTarget(veh, tgt));
                    if      (ch == 0) hsv.h = float(v);
                    else if (ch == 1) hsv.s = v / 100.f;
                    else              hsv.v = v / 100.f;
                    RGB rgb = HsvToRgb(hsv);
                    int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    switch (tgt) {
                    case ColourTarget::Primary:
                        VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                        break;
                    case ColourTarget::Secondary:
                        VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                        VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                        break;
                    case ColourTarget::Neon:
                        VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                        break;
                    case ColourTarget::TyreSmoke:
                        VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rgb.r, rgb.g, rgb.b);
                        break;
                    default: break;
                    }
                });
            }
        };

        class CommandCurrentColourHex : public CommandSlider
        {
            std::atomic<uint32_t> m_packed{0};
            ColourTarget m_tgt;
        public:
            CommandCurrentColourHex(CommandList* parent, ColourTarget tgt)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
                , m_tgt(tgt)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandCurrentColourHex() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                int veh = LscGetVehicle();
                uint32_t p = 0;
                if (veh) {
                    RGB rgb = GetColourByTarget(veh, m_tgt);
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

            void onClick(Click& click) override {
                uint32_t p = m_packed.load(std::memory_order_relaxed);
                if (!(p & 0x80000000u)) return;
                char buf[9];
                snprintf(buf, sizeof(buf), "%06X00", p & 0x00FFFFFFu);
                Rendering::Clipboard::SetText(buf);
                Notifications::Show("Colour", "Copied to clipboard! :D", NotificationType::Success);
            }
        };

        class CommandEnterHexCode : public CommandPhysical
        {
            ColourTarget m_tgt;
        public:
            CommandEnterHexCode(CommandList* parent, ColourTarget tgt)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Enter Hex Code"), CMDNAMES_0(), NOLABEL)
                , m_tgt(tgt) {}

            void onClick(Click& click) override {
                ColourTarget tgt = m_tgt;
                Rendering::MenuCommandBox::Open(
                    "vehcolhex",
                    "Enter Hex Code",
                    "RRGGBB",
                    "",
                    [tgt](const std::string& text) -> bool {
                        if (text.size() != 6) return false;
                        unsigned int r = 0, g = 0, b = 0;
                        if (sscanf(text.c_str(), "%2x%2x%2x", &r, &g, &b) != 3) return false;
                        FiberPool::queueJob([tgt, r, g, b] {
                            int veh = LscGetVehicle(); if (!veh) return;
                            int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                            switch (tgt) {
                            case ColourTarget::Primary:
                                VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, (int)r, (int)g, (int)b);
                                VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                                break;
                            case ColourTarget::Secondary:
                                VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, (int)r, (int)g, (int)b);
                                VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                                break;
                            case ColourTarget::Neon:
                                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, (int)r, (int)g, (int)b);
                                break;
                            case ColourTarget::TyreSmoke:
                                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, (int)r, (int)g, (int)b);
                                break;
                            default: break;
                            }
                        });
                        return true;
                    }
                );
            }
        };

        class CommandNavBarColourSync : public CommandPhysical
        {
            ColourTarget m_tgt;
            bool m_wasActive = false;
        public:
            CommandNavBarColourSync(CommandList* parent, ColourTarget tgt)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), {}, NOLABEL, CMDFLAG_CONCEALED)
                , m_tgt(tgt)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandNavBarColourSync() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                bool active = (Rendering::MenuNavigation::Current() == &Rendering::GridStandCommandList::GetOrCreate(this->parent));
                int veh = LscGetVehicle();
                uint32_t packed = 0u;
                if (veh) {
                    RGB rgb = GetColourByTarget(veh, m_tgt);
                    packed = 0x01000000u | uint32_t(rgb.r) | (uint32_t(rgb.g) << 8) | (uint32_t(rgb.b) << 16);
                }
                static_cast<CommandColourList*>(this->parent)->m_listColour.store(packed, std::memory_order_relaxed);
                if (active) {
                    Rendering::Theme::kNavBarColour.store(packed, std::memory_order_relaxed);
                } else if (m_wasActive) {
                    Rendering::Theme::kNavBarColour.store(0u, std::memory_order_relaxed);
                }
                m_wasActive = active;
            }
        };

        class CommandColourSearch : public CommandPhysical
        {
            std::string m_filter;
            bool m_wasActive = false;
        public:
            explicit CommandColourSearch(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Search"), CMDNAMES_0(), NOLABEL, CMDFLAG_SEARCH_INPUT)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandColourSearch() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {
                bool active = (Rendering::MenuNavigation::Current() == &Rendering::GridStandCommandList::GetOrCreate(this->parent));
                if (m_wasActive && !active && !m_filter.empty()) {
                    m_filter.clear();
                    setMenuName(LIT("Search"));
                    applyFilter();
                }
                m_wasActive = active;
            }

            void onClick(Click& click) override {
                Rendering::MenuCommandBox::Open(
                    "vehcolsearch",
                    "Search Colours",
                    "Filter by name",
                    m_filter,
                    [this](const std::string& text) -> bool {
                        m_filter = text;
                        if (m_filter.empty())
                            setMenuName(LIT("Search"));
                        else
                            setMenuName(Label("Search: " + m_filter, Label::TagLiteral{}));
                        applyFilter();
                        return true;
                    }
                );
            }

        private:
            void applyFilter() {
                std::string lower = m_filter;
                for (char& c : lower) c = (char)std::tolower((unsigned char)c);
                for (auto& child : this->parent->children) {
                    if (child.get() == this) continue;
                    if (!child->isPhysical()) continue;
                    auto* phys = child->getPhysical();
                    if (phys->flags & CMDFLAG_SECTION_HEADER) continue;
                    if (lower.empty()) {
                        phys->flags &= ~CMDFLAG_CONCEALED;
                    } else {
                        std::string name = phys->menu_name.literal_str;
                        for (char& c : name) c = (char)std::tolower((unsigned char)c);
                        if (name.find(lower) != std::string::npos)
                            phys->flags &= ~CMDFLAG_CONCEALED;
                        else
                            phys->flags |= CMDFLAG_CONCEALED;
                    }
                }
                Rendering::GridStandCommandList::GetOrCreate(this->parent).invalidateContent();
            }
        };

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
                    int veh = LscGetVehicle(); if (!veh) return;
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
                    default: break;
                    }
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

        class CommandCopyPrimaryToSecondary : public CommandPhysical
        {
        public:
            explicit CommandCopyPrimaryToSecondary(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Primary Colour"), CMDNAMES_0(), NOLABEL) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    int r, g, b;
                    VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &r, &g, &b);
                    int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                    VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, r, g, b);
                    VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
                });
            }
        };
    }

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

    int LscGetVehicle()
    {
        int ped = PLAYER::GET_PLAYER_PED(-1);
        int veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
        if (veh && ENTITY::DOES_ENTITY_EXIST(veh)) return veh;
        veh = PED::GET_VEHICLE_PED_IS_IN(ped, true);
        return (veh && ENTITY::DOES_ENTITY_EXIST(veh)) ? veh : 0;
    }

    RGB GetVehicleColourRgb(int veh, bool primary)
    {
        int r = 0, g = 0, b = 0;
        if (primary) {
            VEHICLE::GET_VEHICLE_COLOR(veh, &r, &g, &b);
        } else {
            VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &r, &g, &b);
        }
        return {r, g, b};
    }

    RGB GetNeonColourRgb(int veh)
    {
        int r = 0, g = 0, b = 0;
        VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b);
        return {r, g, b};
    }

    RGB GetTyreSmokeColourRgb(int veh)
    {
        int r = 0, g = 0, b = 0;
        VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);
        return {r, g, b};
    }

    void AddStdColours(CommandList* parent, ColourTarget tgt, bool searchable)
    {
        if (searchable)
            parent->createChild<CommandColourSearch>();
        for (auto& [idx, name] : lscClassicColors)
            parent->createChild<CommandStdColourAction>(Label(name, Label::TagLiteral{}), tgt, idx);
    }

    CommandVehRainbow::CommandVehRainbow(CommandList* parent, std::vector<CommandName> cmds,
                                         std::function<void(RGB)> setter)
        : CommandSlider(parent, LIT("Rainbow Mode (0 to 1000)"), std::move(cmds),
              LIT("Cycles colour hue every x milliseconds. 0 = off."), 0, 1000, 0)
        , m_setter(std::move(setter)) {}

    CommandVehRainbow::~CommandVehRainbow()
    {
        if (m_ticking) CommandTickDispatch::RemoveCommand(this);
    }

    void CommandVehRainbow::onChange(Click& click, int)
    {
        if (value > 0 && !m_ticking) {
            CommandTickDispatch::AddCommand(this);
            m_ticking = true;
        } else if (value == 0 && m_ticking) {
            CommandTickDispatch::RemoveCommand(this);
            m_ticking = false;
        }
    }

    void CommandVehRainbow::onTick()
    {
        if (value <= 0) return;
        auto now = get_current_time_millis();
        if (now - m_lastTick < static_cast<time_t>(value)) return;
        m_lastTick = now;
        m_hue = std::fmod(m_hue + 1.f, 360.f);
        m_setter(HsvToRgb({ m_hue, 1.f, 1.f }));
    }

    CommandVehicleColour::CommandVehicleColour(CommandList* parent, Label name,
                                               std::vector<CommandName> cmdnames,
                                               ColourTarget target)
        : CommandColourList(parent, std::move(name), std::move(cmdnames))
    {
        const bool isPrimary = (target == ColourTarget::Primary);

        createChild<CommandNavBarColourSync>(target);

        createChild<CommandVehRainbow>(
            isPrimary ? CMDNAMES("vehprimaryrainbow") : CMDNAMES("vehsecondaryrainbow"),
            [isPrimary](RGB rgb) {
                int veh = LscGetVehicle(); if (!veh) return;
                int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                if (isPrimary) VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                else           VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rgb.r, rgb.g, rgb.b);
                VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
            }
        );

        createChild<CommandVehfinish>(
            isPrimary ? CMDNAMES("vehprimaryfinish") : CMDNAMES("vehsecondaryfinish"),
            [isPrimary](int finish) {
                int veh = LscGetVehicle(); if (!veh) return;
                VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
                int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                if (finish == 5) {
                    int p, s; VEHICLE::GET_VEHICLE_COLOURS(veh, &p, &s);
                    if (isPrimary) VEHICLE::SET_VEHICLE_COLOURS(veh, 120, s);
                    else           VEHICLE::SET_VEHICLE_COLOURS(veh, p, 120);
                } else {
                    if (isPrimary) VEHICLE::SET_VEHICLE_MOD_COLOR_1(veh, finish, 0, 0);
                    else           VEHICLE::SET_VEHICLE_MOD_COLOR_2(veh, finish, 0);
                }
                VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
            }
        );

        auto* lsc = createChild<CommandList>(LIT("LSC Colours"),
            isPrimary ? CMDNAMES("vehprimarylsc") : CMDNAMES("vehsecondarylsc"));
        AddStdColours(lsc, target);
        if (!isPrimary)
            createChild<CommandCopyPrimaryToSecondary>();

        createChild<CommandColourSectionHeader>("HSV Representation");
        createChild<CommandHsvChannel>(LIT("Hue"),        360, 0, target);
        createChild<CommandHsvChannel>(LIT("Saturation"), 100, 1, target);
        createChild<CommandHsvChannel>(LIT("Value"),      100, 2, target);

        createChild<CommandColourSectionHeader>("RGB Representation");
        createChild<CommandColourChannelSlider>(LIT("Red"),
            [isPrimary](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                RGB cur = GetVehicleColourRgb(veh, isPrimary);
                int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                if (isPrimary) VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, v, cur.g, cur.b);
                else           VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, v, cur.g, cur.b);
                VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
            },
            [isPrimary]() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, isPrimary).r; }
        );
        createChild<CommandColourChannelSlider>(LIT("Green"),
            [isPrimary](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                RGB cur = GetVehicleColourRgb(veh, isPrimary);
                int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                if (isPrimary) VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, cur.r, v, cur.b);
                else           VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, cur.r, v, cur.b);
                VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
            },
            [isPrimary]() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, isPrimary).g; }
        );
        createChild<CommandColourChannelSlider>(LIT("Blue"),
            [isPrimary](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                RGB cur = GetVehicleColourRgb(veh, isPrimary);
                int pe, wh; VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pe, &wh);
                if (isPrimary) VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, cur.r, cur.g, v);
                else           VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, cur.r, cur.g, v);
                VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pe, wh);
            },
            [isPrimary]() { int veh = LscGetVehicle(); if (!veh) return 0; return GetVehicleColourRgb(veh, isPrimary).b; }
        );

        createChild<CommandColourSectionHeader>("Other");
        createChild<CommandEnterHexCode>(target);
        createChild<CommandCurrentColourHex>(target);
    }

    CommandNeonColour::CommandNeonColour(CommandList* parent)
        : CommandColourList(parent, LIT("Neon Colour"), CMDNAMES_0())
    {
        createChild<CommandNavBarColourSync>(ColourTarget::Neon);

        createChild<CommandVehRainbow>(
            CMDNAMES("neoncolourrainbow"),
            [](RGB rgb) {
                int veh = LscGetVehicle(); if (!veh) return;
                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, rgb.r, rgb.g, rgb.b);
            }
        );

        createChild<CommandColourSectionHeader>("HSV Representation");
        createChild<CommandHsvChannel>(LIT("Hue"),        360, 0, ColourTarget::Neon);
        createChild<CommandHsvChannel>(LIT("Saturation"), 100, 1, ColourTarget::Neon);
        createChild<CommandHsvChannel>(LIT("Value"),      100, 2, ColourTarget::Neon);

        createChild<CommandColourSectionHeader>("RGB Representation");
        createChild<CommandColourChannelSlider>(LIT("Red"),
            [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, v, g, b);
            },
            []() { int veh = LscGetVehicle(); if (!veh) return 0; int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b); return r; }
        );
        createChild<CommandColourChannelSlider>(LIT("Green"),
            [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, r, v, b);
            },
            []() { int veh = LscGetVehicle(); if (!veh) return 0; int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b); return g; }
        );
        createChild<CommandColourChannelSlider>(LIT("Blue"),
            [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_NEON_COLOUR(veh, r, g, v);
            },
            []() { int veh = LscGetVehicle(); if (!veh) return 0; int r, g, b; VEHICLE::GET_VEHICLE_NEON_COLOUR(veh, &r, &g, &b); return b; }
        );

        createChild<CommandColourSectionHeader>("Other");
        createChild<CommandEnterHexCode>(ColourTarget::Neon);
        createChild<CommandCurrentColourHex>(ColourTarget::Neon);
    }

    CommandTyreSmokeColour::CommandTyreSmokeColour(CommandList* parent)
        : CommandColourList(parent, LIT("Tyre Smoke Colour"), CMDNAMES("vehtire"))
    {
        createChild<CommandNavBarColourSync>(ColourTarget::TyreSmoke);

        createChild<CommandVehRainbow>(
            CMDNAMES("vehtiresmokrainbow"),
            [](RGB rgb) {
                int veh = LscGetVehicle(); if (!veh) return;
                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rgb.r, rgb.g, rgb.b);
            }
        );

        struct CommandIndependenceTires : public CommandToggle {
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
        createChild<CommandIndependenceTires>();

        createChild<CommandColourSectionHeader>("HSV Representation");
        createChild<CommandHsvChannel>(LIT("Hue"),        360, 0, ColourTarget::TyreSmoke);
        createChild<CommandHsvChannel>(LIT("Saturation"), 100, 1, ColourTarget::TyreSmoke);
        createChild<CommandHsvChannel>(LIT("Value"),      100, 2, ColourTarget::TyreSmoke);

        createChild<CommandColourSectionHeader>("RGB Representation");
        createChild<CommandColourChannelSlider>(LIT("Red"),
            [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, v, g, b);
            },
            []() { int veh = LscGetVehicle(); if (!veh) return 0; int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b); return r; }
        );
        createChild<CommandColourChannelSlider>(LIT("Green"),
            [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, r, v, b);
            },
            []() { int veh = LscGetVehicle(); if (!veh) return 0; int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b); return g; }
        );
        createChild<CommandColourChannelSlider>(LIT("Blue"),
            [](int v) {
                int veh = LscGetVehicle(); if (!veh) return;
                int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);
                VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, r, g, v);
            },
            []() { int veh = LscGetVehicle(); if (!veh) return 0; int r, g, b; VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b); return b; }
        );

        createChild<CommandColourSectionHeader>("Other");
        createChild<CommandEnterHexCode>(ColourTarget::TyreSmoke);
        createChild<CommandCurrentColourHex>(ColourTarget::TyreSmoke);

        struct CommandCopyPrimaryToTyreSmoke : public CommandPhysical {
            explicit CommandCopyPrimaryToTyreSmoke(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Copy Primary Colour"), CMDNAMES_0(), NOLABEL) {}

            void onClick(Click& click) override {
                click.ensureScriptThread([] {
                    int veh = LscGetVehicle(); if (!veh) return;
                    RGB rgb = GetVehicleColourRgb(veh, true);
                    VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rgb.r, rgb.g, rgb.b);
                });
            }
        };
        createChild<CommandCopyPrimaryToTyreSmoke>();
    }
}
