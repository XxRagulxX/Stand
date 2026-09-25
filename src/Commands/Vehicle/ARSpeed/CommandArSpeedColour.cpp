#include "Commands/Vehicle/ARSpeed/CommandArSpeedColour.hpp"

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
#include <functional>
#include <string>

namespace Stand
{
    namespace
    {
        class CommandArSpeedSectionHeader : public CommandPhysical
        {
        public:
            CommandArSpeedSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class CommandArSpeedRgbSlider : public CommandSlider
        {
            std::function<void(int)> m_set;
        public:
            CommandArSpeedRgbSlider(CommandList* parent, Label name,
                                    std::function<void(int)> setter, int default_val)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, 255, default_val)
                , m_set(std::move(setter))
            {}

            void onChange(Click& click, int) override { m_set(value); }
        };

        class CommandArSpeedHsvChannel : public CommandSlider
        {
            int m_ch;
            CommandListArSpeedColour* m_owner;
        public:
            CommandArSpeedHsvChannel(CommandList* parent, Label name, int max_val, int ch,
                                     CommandListArSpeedColour* owner)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_ch(ch), m_owner(owner)
            { CommandTickDispatch::AddCommand(this); }
            ~CommandArSpeedHsvChannel() override { CommandTickDispatch::RemoveCommand(this); }

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
                CommandListArSpeedColour* owner = m_owner;
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

        class CommandArSpeedCurrentHex : public CommandSlider
        {
            CommandListArSpeedColour* m_owner;
        public:
            explicit CommandArSpeedCurrentHex(CommandList* parent, CommandListArSpeedColour* owner)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
                , m_owner(owner)
            {}

            std::string getValueText() const override
            {
                char buf[9];
                snprintf(buf, sizeof(buf), "%02X%02X%02X%02X",
                    m_owner->m_r, m_owner->m_g, m_owner->m_b, m_owner->m_a);
                return buf;
            }

            void onClick(Click& click) override
            {
                char buf[9];
                snprintf(buf, sizeof(buf), "%02X%02X%02X%02X",
                    m_owner->m_r, m_owner->m_g, m_owner->m_b, m_owner->m_a);
                Rendering::Clipboard::SetText(buf);
                Notifications::Show("AR Speed Colour", "Copied to clipboard!", NotificationType::Success);
            }
        };

        class CommandArSpeedEnterHex : public CommandPhysical
        {
            CommandListArSpeedColour* m_owner;
        public:
            explicit CommandArSpeedEnterHex(CommandList* parent, CommandListArSpeedColour* owner)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Enter Hex Code"), CMDNAMES_0(), NOLABEL)
                , m_owner(owner) {}

            void onClick(Click& click) override
            {
                CommandListArSpeedColour* owner = m_owner;
                Rendering::MenuCommandBox::Open(
                    "arspeedcolhex",
                    "Enter Hex Code",
                    "RRGGBBAA",
                    "",
                    [owner](const std::string& text) -> bool {
                        unsigned int r = 0, g = 0, b = 0, a = 255;
                        if (text.size() == 6) {
                            if (sscanf(text.c_str(), "%2x%2x%2x", &r, &g, &b) != 3) return false;
                        } else if (text.size() == 8) {
                            if (sscanf(text.c_str(), "%2x%2x%2x%2x", &r, &g, &b, &a) != 4) return false;
                        } else {
                            return false;
                        }
                        owner->m_r = static_cast<int>(r);
                        owner->m_g = static_cast<int>(g);
                        owner->m_b = static_cast<int>(b);
                        owner->m_a = static_cast<int>(a);
                        return true;
                    }
                );
            }
        };
    }

    CommandListArSpeedColour::CommandListArSpeedColour(CommandList* parent)
        : CommandList(parent, LIT("Colour"), CMDNAMES("arspeedhex"))
    {
        createChild<CommandVehRainbow>(
            CMDNAMES("arspeedrainbow"),
            [this](RGB rgb) { m_r = rgb.r; m_g = rgb.g; m_b = rgb.b; }
        );

        createChild<CommandArSpeedSectionHeader>("HSV Representation");
        createChild<CommandArSpeedHsvChannel>(LIT("Hue"),        360, 0, this);
        createChild<CommandArSpeedHsvChannel>(LIT("Saturation"), 100, 1, this);
        createChild<CommandArSpeedHsvChannel>(LIT("Value"),      100, 2, this);

        createChild<CommandArSpeedSectionHeader>("RGB Representation");
        createChild<CommandArSpeedRgbSlider>(LIT("Red"),   [this](int v) { m_r = v; }, m_r);
        createChild<CommandArSpeedRgbSlider>(LIT("Green"), [this](int v) { m_g = v; }, m_g);
        createChild<CommandArSpeedRgbSlider>(LIT("Blue"),  [this](int v) { m_b = v; }, m_b);
        createChild<CommandArSpeedRgbSlider>(LIT("Alpha"), [this](int v) { m_a = v; }, m_a);

        createChild<CommandArSpeedSectionHeader>("Other");
        createChild<CommandArSpeedEnterHex>(this);
        createChild<CommandArSpeedCurrentHex>(this);
    }
}
