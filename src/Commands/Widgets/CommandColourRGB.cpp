#include "Commands/Widgets/CommandColourRGB.hpp"

#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/Notifications.hpp"
#include "Util/Label.hpp"

#include <cmath>
#include <cstdio>

namespace Stand
{
    namespace
    {
        class ColourSectionHeader : public CommandPhysical
        {
        public:
            ColourSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class ColourHsvChannel : public CommandSlider
        {
            int m_ch;
            CommandColourRGB* m_owner;
        public:
            ColourHsvChannel(CommandList* parent, Label name, int max_val, int ch, CommandColourRGB* owner)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_ch(ch), m_owner(owner)
            { CommandTickDispatch::AddCommand(this); }

            ~ColourHsvChannel() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                HSV hsv = RgbToHsv({ m_owner->m_r, m_owner->m_g, m_owner->m_b });
                int v;
                if      (m_ch == 0) v = static_cast<int>(std::round(hsv.h));
                else if (m_ch == 1) v = static_cast<int>(std::round(hsv.s * 100.f));
                else                v = static_cast<int>(std::round(hsv.v * 100.f));
                Click click(CLICK_AUTO, TC_SCRIPT_YIELDABLE);
                setValueIndicator(click, v);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                int v = value; int ch = m_ch;
                CommandColourRGB* owner = m_owner;
                click.ensureScriptThread([v, ch, owner] {
                    HSV hsv = RgbToHsv({ owner->m_r, owner->m_g, owner->m_b });
                    if      (ch == 0) hsv.h = static_cast<float>(v);
                    else if (ch == 1) hsv.s = v / 100.f;
                    else              hsv.v = v / 100.f;
                    RGB out = HsvToRgb(hsv);
                    owner->m_r = out.r; owner->m_g = out.g; owner->m_b = out.b;
                });
            }
        };

        class ColourRgbSlider : public CommandSlider
        {
            int* m_ch;
        public:
            ColourRgbSlider(CommandList* parent, Label name, int* ch, int default_val)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, 255, default_val)
                , m_ch(ch) {}

            void onChange(Click& click, int) override { *m_ch = value; }
        };

        class ColourCurrentHex : public CommandSlider
        {
            CommandColourRGB* m_owner;
        public:
            explicit ColourCurrentHex(CommandList* parent, CommandColourRGB* owner)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
                , m_owner(owner) {}

            std::string getValueText() const override
            {
                char buf[7];
                snprintf(buf, sizeof(buf), "%02X%02X%02X",
                    m_owner->m_r, m_owner->m_g, m_owner->m_b);
                return buf;
            }

            void onClick(Click& click) override
            {
                char buf[7];
                snprintf(buf, sizeof(buf), "%02X%02X%02X",
                    m_owner->m_r, m_owner->m_g, m_owner->m_b);
                Rendering::Clipboard::SetText(buf);
                Notifications::Show("Colour", "Copied to clipboard!", NotificationType::Success);
            }
        };

        class ColourEnterHex : public CommandPhysical
        {
            CommandColourRGB* m_owner;
        public:
            explicit ColourEnterHex(CommandList* parent, CommandColourRGB* owner)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Enter Hex Code"), CMDNAMES_0(), NOLABEL)
                , m_owner(owner) {}

            void onClick(Click& click) override
            {
                CommandColourRGB* owner = m_owner;
                Rendering::MenuCommandBox::Open(
                    "colrgbhex",
                    "Enter Hex Code",
                    "RRGGBB",
                    "",
                    [owner](const std::string& text) -> bool {
                        if (text.size() != 6) return false;
                        unsigned int r = 0, g = 0, b = 0;
                        if (sscanf(text.c_str(), "%2x%2x%2x", &r, &g, &b) != 3) return false;
                        owner->m_r = static_cast<int>(r);
                        owner->m_g = static_cast<int>(g);
                        owner->m_b = static_cast<int>(b);
                        return true;
                    }
                );
            }
        };
    }

    CommandColourRGB::CommandColourRGB(CommandList* parent,
        Label&& menu_name,
        std::vector<CommandName>&& command_names,
        Label&& help_text,
        int default_r,
        int default_g,
        int default_b)
        : CommandList(parent, std::move(menu_name), std::move(command_names), std::move(help_text))
        , m_r(default_r), m_g(default_g), m_b(default_b)
    {
        createChild<ColourSectionHeader>("HSV Representation");
        createChild<ColourHsvChannel>(LIT("Hue"),        360, 0, this);
        createChild<ColourHsvChannel>(LIT("Saturation"), 100, 1, this);
        createChild<ColourHsvChannel>(LIT("Value"),      100, 2, this);

        createChild<ColourSectionHeader>("RGB Representation");
        createChild<ColourRgbSlider>(LIT("Red"),   &m_r, m_r);
        createChild<ColourRgbSlider>(LIT("Green"), &m_g, m_g);
        createChild<ColourRgbSlider>(LIT("Blue"),  &m_b, m_b);

        createChild<ColourSectionHeader>("Other");
        createChild<ColourEnterHex>(this);
        createChild<ColourCurrentHex>(this);
    }
}
