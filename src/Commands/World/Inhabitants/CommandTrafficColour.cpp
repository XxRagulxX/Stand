#include "Commands/World/Inhabitants/CommandTrafficColour.hpp"

#include "Commands/Vehicle/LSC/CommandVehicleColour.hpp"
#include "Commands/Widgets/CommandFlags.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"
#include "Game/Pools.hpp"
#include "Menu/Click.hpp"
#include "Rendering/Clipboard.hpp"
#include "Rendering/GridStandCommandList.hpp"
#include "Rendering/MenuCommandBox.hpp"
#include "Rendering/MenuNavigation.hpp"
#include "Rendering/Notifications.hpp"
#include "Rendering/Theme.hpp"
#include "Scripting/FiberPool.hpp"
#include "Scripting/Natives.hpp"
#include "Util/Label.hpp"
#include "World/Self.hpp"

#include <cmath>
#include <cstdio>
#include <functional>
#include <string>

namespace Stand
{
    namespace
    {
        class CommandTrafficColourSectionHeader : public CommandPhysical
        {
        public:
            CommandTrafficColourSectionHeader(CommandList* parent, const char* label)
                : CommandPhysical(COMMAND_ACTION, parent, Label(label, Label::TagLiteral{}), {}, NOLABEL, CMDFLAG_SECTION_HEADER) {}
        };

        class CommandTrafficColourMain : public CommandToggle
        {
            CommandTrafficColour* m_owner;
        public:
            explicit CommandTrafficColourMain(CommandList* parent)
                : CommandToggle(parent, LIT("Traffic Colour"), CMDNAMES("colourtraffic", "colortraffic"))
                , m_owner(static_cast<CommandTrafficColour*>(parent))
            {}

            void onEnable(Click& click) final
            {
                CommandTickDispatch::AddCommand(this);
            }

            void onDisable(Click& click) final
            {
                CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() final
            {
                int tr = m_owner->m_r, tg = m_owner->m_g, tb = m_owner->m_b;
                int player_veh = Self::GetVehicle().GetHandle();
                for (auto veh : Pools::GetVehicles())
                {
                    if (!veh || veh.GetHandle() == player_veh) continue;
                    if (DECORATOR::DECOR_EXIST_ON(veh.GetHandle(), "Player_Vehicle")) continue;
                    int cr = 0, cg = 0, cb = 0;
                    VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh.GetHandle(), &cr, &cg, &cb);
                    if (cr == tr && cg == tg && cb == tb) continue;
                    if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh.GetHandle()))
                    {
                        VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh.GetHandle(), tr, tg, tb);
                        VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh.GetHandle(), tr, tg, tb);
                    }
                    else
                    {
                        NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh.GetHandle());
                    }
                }
            }

            ~CommandTrafficColourMain() override
            {
                CommandTickDispatch::RemoveCommand(this);
            }
        };

        class CommandTrafficNavBarSync : public CommandPhysical
        {
            CommandTrafficColour* m_owner;
            bool m_wasActive = false;
        public:
            explicit CommandTrafficNavBarSync(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT(""), {}, NOLABEL, CMDFLAG_CONCEALED)
                , m_owner(static_cast<CommandTrafficColour*>(parent))
            { CommandTickDispatch::AddCommand(this); }
            ~CommandTrafficNavBarSync() override { CommandTickDispatch::RemoveCommand(this); }

            void onTick() override
            {
                bool active = (Rendering::MenuNavigation::Current() == &Rendering::GridStandCommandList::GetOrCreate(this->parent));
                uint32_t packed = 0x01000000u
                    | uint32_t(m_owner->m_r)
                    | (uint32_t(m_owner->m_g) << 8)
                    | (uint32_t(m_owner->m_b) << 16);
                m_owner->m_listColour.store(packed, std::memory_order_relaxed);
                if (active)
                    Rendering::Theme::kNavBarColour.store(packed, std::memory_order_relaxed);
                else if (m_wasActive)
                    Rendering::Theme::kNavBarColour.store(0u, std::memory_order_relaxed);
                m_wasActive = active;
            }
        };

        class CommandTrafficHsvChannel : public CommandSlider
        {
            int m_ch;
            CommandTrafficColour* m_owner;
        public:
            CommandTrafficHsvChannel(CommandList* parent, Label name, int max_val, int ch)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, max_val, 0)
                , m_ch(ch), m_owner(static_cast<CommandTrafficColour*>(parent))
            { CommandTickDispatch::AddCommand(this); }
            ~CommandTrafficHsvChannel() override { CommandTickDispatch::RemoveCommand(this); }

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
                CommandTrafficColour* owner = m_owner;
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

        class CommandTrafficRgbSlider : public CommandSlider
        {
            std::function<void(int)> m_set;
        public:
            CommandTrafficRgbSlider(CommandList* parent, Label name,
                                    std::function<void(int)> setter, int default_val)
                : CommandSlider(parent, std::move(name), CMDNAMES_0(), NOLABEL, 0, 255, default_val)
                , m_set(std::move(setter))
            {}

            void onChange(Click& click, int) override { m_set(value); }
        };

        class CommandTrafficCurrentHex : public CommandSlider
        {
            CommandTrafficColour* m_owner;
        public:
            explicit CommandTrafficCurrentHex(CommandList* parent)
                : CommandSlider(parent, LIT("Current Colour (Hex)"), CMDNAMES_0(), NOLABEL, 0, 0, 0, 1)
                , m_owner(static_cast<CommandTrafficColour*>(parent))
            {}

            std::string getValueText() const override
            {
                char buf[8];
                snprintf(buf, sizeof(buf), "%02X%02X%02X", m_owner->m_r, m_owner->m_g, m_owner->m_b);
                return buf;
            }

            void onClick(Click& click) override
            {
                char buf[8];
                snprintf(buf, sizeof(buf), "%02X%02X%02X", m_owner->m_r, m_owner->m_g, m_owner->m_b);
                Rendering::Clipboard::SetText(buf);
                Notifications::Show("Traffic Colour", "Copied to clipboard!", NotificationType::Success);
            }
        };

        class CommandTrafficEnterHex : public CommandPhysical
        {
            CommandTrafficColour* m_owner;
        public:
            explicit CommandTrafficEnterHex(CommandList* parent)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Enter Hex Code"), CMDNAMES_0(), NOLABEL)
                , m_owner(static_cast<CommandTrafficColour*>(parent))
            {}

            void onClick(Click& click) override
            {
                CommandTrafficColour* owner = m_owner;
                Rendering::MenuCommandBox::Open(
                    "trafficcolhex",
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

    CommandTrafficColour::CommandTrafficColour(CommandList* parent)
        : CommandColourList(parent, LIT("Traffic Colour"))
    {
        createChild<CommandTrafficNavBarSync>();
        createChild<CommandTrafficColourMain>();

        createChild<CommandVehRainbow>(
            CMDNAMES("trafficcolourrainbow"),
            [this](RGB rgb) { m_r = rgb.r; m_g = rgb.g; m_b = rgb.b; }
        );

        createChild<CommandTrafficColourSectionHeader>("HSV Representation");
        createChild<CommandTrafficHsvChannel>(LIT("Hue"),        360, 0);
        createChild<CommandTrafficHsvChannel>(LIT("Saturation"), 100, 1);
        createChild<CommandTrafficHsvChannel>(LIT("Value"),      100, 2);

        createChild<CommandTrafficColourSectionHeader>("RGB Representation");
        createChild<CommandTrafficRgbSlider>(LIT("Red"),   [this](int v) { m_r = v; }, m_r);
        createChild<CommandTrafficRgbSlider>(LIT("Green"), [this](int v) { m_g = v; }, m_g);
        createChild<CommandTrafficRgbSlider>(LIT("Blue"),  [this](int v) { m_b = v; }, m_b);

        createChild<CommandTrafficColourSectionHeader>("Other");
        createChild<CommandTrafficEnterHex>();
        createChild<CommandTrafficCurrentHex>();
    }
}
