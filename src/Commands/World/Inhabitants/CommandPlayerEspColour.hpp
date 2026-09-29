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
        struct PlayerRGB { int r, g, b; };
        struct PlayerHSV { float h, s, v; };

        PlayerHSV PlayerRgbToHsv(PlayerRGB c)
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

        PlayerRGB PlayerHsvToRgb(PlayerHSV c)
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

        PlayerRGB PlayerEspGetRgb()
        {
            return { AllEntitiesEveryTick::player_esp_colour_r,
                     AllEntitiesEveryTick::player_esp_colour_g,
                     AllEntitiesEveryTick::player_esp_colour_b };
        }

        void PlayerEspSetRgb(int r, int g, int b)
        {
            AllEntitiesEveryTick::player_esp_colour_r = r;
            AllEntitiesEveryTick::player_esp_colour_g = g;
            AllEntitiesEveryTick::player_esp_colour_b = b;
        }

        class PlayerEspSectionHeader : public CommandPhysical
        {
        public:
            PlayerEspSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class PlayerEspNavBarSync : public CommandPhysical
        {
            bool m_wasActive = false;
        public:
            explicit PlayerEspNavBarSync(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), {}, NOLABEL, CMDFLAG_CONCEALED)
            { CommandTickDispatch::AddCommand(this); }
            ~PlayerEspNavBarSync() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                bool active = (Rendering::MenuNavigation::Current() == &Rendering::GridStandCommandList::GetOrCreate(this->parent));
                auto rgb = PlayerEspGetRgb();
                uint32_t packed = 0x01000000u | uint32_t(rgb.r) | (uint32_t(rgb.g) << 8) | (uint32_t(rgb.b) << 16);
                if (active) {
                    Rendering::Theme::kNavBarColour.store(packed, std::memory_order_relaxed);
                } else if (m_wasActive) {
                    Rendering::Theme::kNavBarColour.store(0u, std::memory_order_relaxed);
                }
                m_wasActive = active;
            }
        };

        class PlayerEspHsvChannel : public CommandSlider
        {
            int m_ch;
        public:
            PlayerEspHsvChannel(CommandList* parent, Label name, int max_val, int ch)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_ch(ch)
            { CommandTickDispatch::AddCommand(this); }
            ~PlayerEspHsvChannel() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                PlayerHSV hsv = PlayerRgbToHsv(PlayerEspGetRgb());
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
                PlayerHSV hsv = PlayerRgbToHsv(PlayerEspGetRgb());
                if      (ch == 0) hsv.h = float(v);
                else if (ch == 1) hsv.s = v / 100.f;
                else              hsv.v = v / 100.f;
                PlayerRGB rgb = PlayerHsvToRgb(hsv);
                PlayerEspSetRgb(rgb.r, rgb.g, rgb.b);
            }
        };

        class PlayerEspRgbSlider : public CommandSlider
        {
            int m_ch;
        public:
            PlayerEspRgbSlider(CommandList* parent, Label name, int ch)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, 255, 0)
                , m_ch(ch)
            { CommandTickDispatch::AddCommand(this); }
            ~PlayerEspRgbSlider() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                auto rgb = PlayerEspGetRgb();
                int v = (m_ch == 0) ? rgb.r : (m_ch == 1) ? rgb.g : rgb.b;
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, v);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                auto rgb = PlayerEspGetRgb();
                if      (m_ch == 0) rgb.r = value;
                else if (m_ch == 1) rgb.g = value;
                else                rgb.b = value;
                PlayerEspSetRgb(rgb.r, rgb.g, rgb.b);
            }
        };

        class PlayerEspHexDisplay : public CommandSlider
        {
        public:
            explicit PlayerEspHexDisplay(CommandList* parent)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
            { CommandTickDispatch::AddCommand(this); }
            ~PlayerEspHexDisplay() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {}

            std::string getValueText() const override
            {
                auto rgb = PlayerEspGetRgb();
                char buf[9];
                snprintf(buf, sizeof(buf), "%02X%02X%02X00", rgb.r, rgb.g, rgb.b);
                return buf;
            }

            void onClick(Click& click) override
            {
                auto rgb = PlayerEspGetRgb();
                char buf[9];
                snprintf(buf, sizeof(buf), "%02X%02X%02X00", rgb.r, rgb.g, rgb.b);
                Rendering::Clipboard::SetText(buf);
                Notifications::Show("Colour", "Copied to clipboard! :D", NotificationType::Success);
            }
        };

        class PlayerEspEnterHex : public CommandPhysical
        {
        public:
            explicit PlayerEspEnterHex(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Enter Hex Code"), CMDNAMES_0(), NOLABEL) {}

            void onClick(Click& click) override
            {
                Rendering::MenuCommandBox::Open(
                    "playerespcolhex",
                    "Enter Hex Code",
                    "RRGGBB",
                    "",
                    [](const std::string& text) -> bool {
                        if (text.size() != 6) return false;
                        unsigned int r = 0, g = 0, b = 0;
                        if (sscanf(text.c_str(), "%2x%2x%2x", &r, &g, &b) != 3) return false;
                        PlayerEspSetRgb((int)r, (int)g, (int)b);
                        return true;
                    }
                );
            }
        };
    }

    class CommandPlayerEspColour : public CommandList
    {
    public:
        explicit CommandPlayerEspColour(CommandList* parent)
            : CommandList(parent, LIT("Default Colour"), CMDNAMES("playerespcolour"))
        {
            createChild<PlayerEspNavBarSync>();

            createChild<PlayerEspSectionHeader>("HSV Representation");
            createChild<PlayerEspHsvChannel>(LIT("Hue"),        360, 0);
            createChild<PlayerEspHsvChannel>(LIT("Saturation"), 100, 1);
            createChild<PlayerEspHsvChannel>(LIT("Value"),      100, 2);

            createChild<PlayerEspSectionHeader>("RGB Representation");
            createChild<PlayerEspRgbSlider>(LIT("Red"),   0);
            createChild<PlayerEspRgbSlider>(LIT("Green"), 1);
            createChild<PlayerEspRgbSlider>(LIT("Blue"),  2);

            createChild<PlayerEspSectionHeader>("Other");
            createChild<PlayerEspEnterHex>();
            createChild<PlayerEspHexDisplay>();
        }
    };
}
