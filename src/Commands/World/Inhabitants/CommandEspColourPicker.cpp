#include "Commands/World/Inhabitants/CommandEspColourPicker.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Util/Label.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>

namespace Stand
{
    namespace
    {
        struct EcpRGB { int r, g, b; };
        struct EcpHSV { float h, s, v; };

        EcpHSV EcpRgbToHsv(EcpRGB c)
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

        EcpRGB EcpHsvToRgb(EcpHSV c)
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

        class EcpSectionHeader : public CommandPhysical
        {
        public:
            EcpSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class EcpNavBarSync : public CommandPhysical
        {
            int* m_r; int* m_g; int* m_b;
            bool m_wasActive = false;
        public:
            EcpNavBarSync(CommandList* parent, int* r, int* g, int* b)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), {}, NOLABEL, CMDFLAG_CONCEALED)
                , m_r(r), m_g(g), m_b(b)
            { CommandTickDispatch::AddCommand(this); }
            ~EcpNavBarSync() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                bool active = (Rendering::MenuNavigation::Current() == &Rendering::GridStandCommandList::GetOrCreate(this->parent));
                uint32_t packed = 0x01000000u | uint32_t(*m_r) | (uint32_t(*m_g) << 8) | (uint32_t(*m_b) << 16);
                if (active) {
                    Rendering::Theme::kNavBarColour.store(packed, std::memory_order_relaxed);
                } else if (m_wasActive) {
                    Rendering::Theme::kNavBarColour.store(0u, std::memory_order_relaxed);
                }
                m_wasActive = active;
            }
        };

        class EcpHsvChannel : public CommandSlider
        {
            int* m_r; int* m_g; int* m_b;
            int  m_ch;
        public:
            EcpHsvChannel(CommandList* parent, Label name, int max_val, int ch, int* r, int* g, int* b)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_r(r), m_g(g), m_b(b), m_ch(ch)
            { CommandTickDispatch::AddCommand(this); }
            ~EcpHsvChannel() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                EcpHSV hsv = EcpRgbToHsv({*m_r, *m_g, *m_b});
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
                EcpHSV hsv = EcpRgbToHsv({*m_r, *m_g, *m_b});
                if      (m_ch == 0) hsv.h = float(value);
                else if (m_ch == 1) hsv.s = value / 100.f;
                else                hsv.v = value / 100.f;
                EcpRGB rgb = EcpHsvToRgb(hsv);
                *m_r = rgb.r; *m_g = rgb.g; *m_b = rgb.b;
            }
        };

        class EcpRgbSlider : public CommandSlider
        {
            int* m_field;
            int* m_r; int* m_g; int* m_b;
            int  m_ch;
        public:
            EcpRgbSlider(CommandList* parent, Label name, int ch, int* r, int* g, int* b)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, 255, 0)
                , m_r(r), m_g(g), m_b(b), m_ch(ch)
            {
                m_field = (ch == 0) ? r : (ch == 1) ? g : b;
                CommandTickDispatch::AddCommand(this);
            }
            ~EcpRgbSlider() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, *m_field);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                *m_field = value;
            }
        };

        class EcpHexDisplay : public CommandSlider
        {
            int* m_r; int* m_g; int* m_b;
        public:
            EcpHexDisplay(CommandList* parent, int* r, int* g, int* b)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
                , m_r(r), m_g(g), m_b(b)
            { CommandTickDispatch::AddCommand(this); }
            ~EcpHexDisplay() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override {}

            std::string getValueText() const override
            {
                char buf[9];
                snprintf(buf, sizeof(buf), "%02X%02X%02X00", *m_r, *m_g, *m_b);
                return buf;
            }

            void onClick(Click& click) override
            {
                char buf[9];
                snprintf(buf, sizeof(buf), "%02X%02X%02X00", *m_r, *m_g, *m_b);
                Rendering::Clipboard::SetText(buf);
                Notifications::Show("Colour", "Copied to clipboard! :D", NotificationType::Success);
            }
        };

        class EcpEnterHex : public CommandPhysical
        {
            int* m_r; int* m_g; int* m_b;
        public:
            EcpEnterHex(CommandList* parent, int* r, int* g, int* b)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Enter Hex Code"), CMDNAMES_0(), NOLABEL)
                , m_r(r), m_g(g), m_b(b) {}

            void onClick(Click& click) override
            {
                int* r = m_r; int* g = m_g; int* b = m_b;
                Rendering::MenuCommandBox::Open(
                    "ecpcolhex",
                    "Enter Hex Code",
                    "RRGGBB",
                    "",
                    [r, g, b](const std::string& text) -> bool {
                        if (text.size() != 6) return false;
                        unsigned int rv = 0, gv = 0, bv = 0;
                        if (sscanf(text.c_str(), "%2x%2x%2x", &rv, &gv, &bv) != 3) return false;
                        *r = (int)rv; *g = (int)gv; *b = (int)bv;
                        return true;
                    }
                );
            }
        };
    }

    CommandEspColourPicker::CommandEspColourPicker(CommandList* parent, Label name,
        std::vector<CommandName> cmdnames, int* r, int* g, int* b)
        : CommandList(parent, std::move(name), std::move(cmdnames))
    {
        createChild<EcpNavBarSync>(r, g, b);

        createChild<EcpSectionHeader>("HSV Representation");
        createChild<EcpHsvChannel>(LIT("Hue"),        360, 0, r, g, b);
        createChild<EcpHsvChannel>(LIT("Saturation"), 100, 1, r, g, b);
        createChild<EcpHsvChannel>(LIT("Value"),      100, 2, r, g, b);

        createChild<EcpSectionHeader>("RGB Representation");
        createChild<EcpRgbSlider>(LIT("Red"),   0, r, g, b);
        createChild<EcpRgbSlider>(LIT("Green"), 1, r, g, b);
        createChild<EcpRgbSlider>(LIT("Blue"),  2, r, g, b);

        createChild<EcpSectionHeader>("Other");
        createChild<EcpEnterHex>(r, g, b);
        createChild<EcpHexDisplay>(r, g, b);
    }
}
