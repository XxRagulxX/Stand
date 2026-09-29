#pragma once
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Game/AllEntitiesEveryTick.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Label.hpp"

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <string>

namespace Stand
{
    namespace
    {
        struct NpcRGB { int r, g, b; };
        struct NpcHSV { float h, s, v; };

        NpcHSV NpcRgbToHsv(NpcRGB c)
        {
            float r = c.r / 255.f, g = c.g / 255.f, b = c.b / 255.f;
            float mx = std::max({r, g, b}), mn = std::min({r, g, b});
            float d = mx - mn, h = 0.f;
            if (d > 0.f) {
                if      (mx == r) h = 60.f * std::fmod((g - b) / d, 6.f);
                else if (mx == g) h = 60.f * ((b - r) / d + 2.f);
                else              h = 60.f * ((r - g) / d + 4.f);
                if (h < 0.f) h += 360.f;
            }
            return { h, mx > 0.f ? d / mx : 0.f, mx };
        }

        NpcRGB NpcHsvToRgb(NpcHSV c)
        {
            float x = c.s * c.v, m = c.v - x;
            float k = c.h / 60.f, f = k - std::floor(k);
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

        NpcRGB NpcEspGetRgb()
        {
            return { AllEntitiesEveryTick::npc_esp_colour_r,
                     AllEntitiesEveryTick::npc_esp_colour_g,
                     AllEntitiesEveryTick::npc_esp_colour_b };
        }

        void NpcEspSetRgb(int r, int g, int b)
        {
            AllEntitiesEveryTick::npc_esp_colour_r = r;
            AllEntitiesEveryTick::npc_esp_colour_g = g;
            AllEntitiesEveryTick::npc_esp_colour_b = b;
        }

        class NpcEspSectionHeader : public CommandPhysical
        {
        public:
            NpcEspSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class NpcEspNavBarSync : public CommandPhysical
        {
            bool m_wasActive = false;
        public:
            explicit NpcEspNavBarSync(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), {}, NOLABEL, CMDFLAG_CONCEALED)
            { CommandTickDispatch::AddCommand(this); }
            ~NpcEspNavBarSync() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                bool active = (Rendering::MenuNavigation::Current() == &Rendering::GridStandCommandList::GetOrCreate(this->parent));
                auto rgb = NpcEspGetRgb();
                uint32_t packed = 0x01000000u | uint32_t(rgb.r) | (uint32_t(rgb.g) << 8) | (uint32_t(rgb.b) << 16);
                if (active) {
                    Rendering::Theme::kNavBarColour.store(packed, std::memory_order_relaxed);
                } else if (m_wasActive) {
                    Rendering::Theme::kNavBarColour.store(0u, std::memory_order_relaxed);
                }
                m_wasActive = active;
            }
        };

        class NpcEspHsvChannel : public CommandSlider
        {
            int m_ch;
        public:
            NpcEspHsvChannel(CommandList* parent, Label name, int max_val, int ch)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_ch(ch)
            { CommandTickDispatch::AddCommand(this); }
            ~NpcEspHsvChannel() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                NpcHSV hsv = NpcRgbToHsv(NpcEspGetRgb());
                int v;
                if      (m_ch == 0) v = int(std::round(hsv.h));
                else if (m_ch == 1) v = int(std::round(hsv.s * 100.f));
                else                v = int(std::round(hsv.v * 100.f));
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, v);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                int v = value, ch = m_ch;
                NpcHSV hsv = NpcRgbToHsv(NpcEspGetRgb());
                if      (ch == 0) hsv.h = float(v);
                else if (ch == 1) hsv.s = v / 100.f;
                else              hsv.v = v / 100.f;
                NpcRGB rgb = NpcHsvToRgb(hsv);
                NpcEspSetRgb(rgb.r, rgb.g, rgb.b);
            }
        };

        class NpcEspRgbSlider : public CommandSlider
        {
            int m_ch;
        public:
            NpcEspRgbSlider(CommandList* parent, Label name, int ch)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, 255, 0)
                , m_ch(ch)
            { CommandTickDispatch::AddCommand(this); }
            ~NpcEspRgbSlider() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                auto rgb = NpcEspGetRgb();
                int v = (m_ch == 0) ? rgb.r : (m_ch == 1) ? rgb.g : rgb.b;
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, v);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                auto rgb = NpcEspGetRgb();
                if      (m_ch == 0) rgb.r = value;
                else if (m_ch == 1) rgb.g = value;
                else                rgb.b = value;
                NpcEspSetRgb(rgb.r, rgb.g, rgb.b);
            }
        };

        class NpcEspHexDisplay : public CommandSlider
        {
        public:
            explicit NpcEspHexDisplay(CommandList* parent)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
            { CommandTickDispatch::AddCommand(this); }
            ~NpcEspHexDisplay() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {}

            std::string getValueText() const override
            {
                auto rgb = NpcEspGetRgb();
                char buf[9];
                snprintf(buf, sizeof(buf), "%02X%02X%02X00", rgb.r, rgb.g, rgb.b);
                return buf;
            }

            void onClick(Click& click) override
            {
                auto rgb = NpcEspGetRgb();
                char buf[9];
                snprintf(buf, sizeof(buf), "%02X%02X%02X00", rgb.r, rgb.g, rgb.b);
                Rendering::Clipboard::SetText(buf);
                Notifications::Show("Colour", "Copied to clipboard! :D", NotificationType::Success);
            }
        };

        class NpcEspEnterHex : public CommandPhysical
        {
        public:
            explicit NpcEspEnterHex(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Enter Hex Code"), CMDNAMES_0(), NOLABEL) {}

            void onClick(Click& click) override
            {
                Rendering::MenuCommandBox::Open(
                    "npcespcolhex",
                    "Enter Hex Code",
                    "RRGGBB",
                    "",
                    [](const std::string& text) -> bool {
                        if (text.size() != 6) return false;
                        unsigned int r = 0, g = 0, b = 0;
                        if (sscanf(text.c_str(), "%2x%2x%2x", &r, &g, &b) != 3) return false;
                        NpcEspSetRgb((int)r, (int)g, (int)b);
                        return true;
                    }
                );
            }
        };
    }

    class CommandNpcEspColour : public CommandList
    {
    public:
        explicit CommandNpcEspColour(CommandList* parent)
            : CommandList(parent, LIT("Colour"), CMDNAMES("npcespcolour"))
        {
            createChild<NpcEspNavBarSync>();

            createChild<NpcEspSectionHeader>("HSV Representation");
            createChild<NpcEspHsvChannel>(LIT("Hue"),        360, 0);
            createChild<NpcEspHsvChannel>(LIT("Saturation"), 100, 1);
            createChild<NpcEspHsvChannel>(LIT("Value"),      100, 2);

            createChild<NpcEspSectionHeader>("RGB Representation");
            createChild<NpcEspRgbSlider>(LIT("Red"),   0);
            createChild<NpcEspRgbSlider>(LIT("Green"), 1);
            createChild<NpcEspRgbSlider>(LIT("Blue"),  2);

            createChild<NpcEspSectionHeader>("Other");
            createChild<NpcEspEnterHex>();
            createChild<NpcEspHexDisplay>();
        }
    };
}
