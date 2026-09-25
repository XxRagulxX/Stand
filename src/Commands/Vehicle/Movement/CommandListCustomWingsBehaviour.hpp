#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandPhysical.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandCustomWingsBehaviour : public CommandToggle
        {
            float m_pos = 0.f;
            bool  m_dir = true;
        public:
            CommandSlider* m_min   = nullptr;
            CommandSlider* m_max   = nullptr;
            CommandSlider* m_speed = nullptr;

            explicit CommandCustomWingsBehaviour(CommandList* parent)
                : CommandToggle(parent, LIT("Custom Wings Behaviour"), CMDNAMES("wingsmod"),
                    LIT("Allows you to customise the behaviour of the wings on the original oppressor.")) {}

            void onChange(Click& click) override
            {
                if (!m_on) return;
                m_pos = m_min ? (float)m_min->value / 15.f : 0.f;
                m_dir = true;
                onChangeToggleScriptTickEventHandler(click, [this] {
                    if (!m_on) return false;
                    int veh = MovVehicle();
                    if (!veh) return true;
                    float mn  = m_min  ? (float)m_min->value  / 15.f : 0.f;
                    float mx  = m_max  ? (float)m_max->value  / 15.f : 1.f;
                    float spd = m_speed ? (float)m_speed->value / 300.f : 0.017f;
                    if (mx < mn) mx = mn;
                    if (m_dir)
                    {
                        m_pos += spd;
                        if (m_pos >= mx) { m_pos = mx; m_dir = false; }
                    }
                    else
                    {
                        m_pos -= spd;
                        if (m_pos <= mn) { m_pos = mn; m_dir = true; }
                    }
                    VEHICLE::SET_VEHICLE_FLIGHT_NOZZLE_POSITION(veh, m_pos);
                    return true;
                });
            }
        };

        class CommandWingsSetSpeedFromMax : public CommandPhysical
        {
            CommandSlider* m_max;
            CommandSlider* m_speed;
        public:
            CommandWingsSetSpeedFromMax(CommandList* parent, CommandSlider* max_s, CommandSlider* spd_s)
                : CommandPhysical(COMMAND_ACTION, parent, LIT("Set Speed Based On Max Position"), CMDNAMES_0(), NOLABEL)
                , m_max(max_s), m_speed(spd_s) {}

            void onClick(Click& click) override
            {
                auto* mx = m_max; auto* spd = m_speed;
                click.ensureScriptThread([mx, spd] {
                    if (!mx || !spd) return;
                    int new_val = mx->value / 3;
                    if (spd->value != new_val)
                    {
                        int prev = spd->value;
                        spd->value = new_val;
                        Click ac(CLICK_AUTO, TC_SCRIPT_NOYIELD);
                        spd->onChange(ac, prev);
                    }
                });
            }
        };

        class CommandListCustomWingsBehaviour : public CommandList
        {
        public:
            explicit CommandListCustomWingsBehaviour(CommandList* parent)
                : CommandList(parent, LIT("Custom Wings Behaviour"), CMDNAMES_0())
            {
                auto* tog = createChild<CommandCustomWingsBehaviour>();
                auto* mn  = createChild<CommandSlider>(LIT("Min Position"), CMDNAMES("wingsminpos"), NOLABEL, 0, 15,  0, 1);
                auto* mx  = createChild<CommandSlider>(LIT("Max Position"), CMDNAMES("wingsmaxpos"), NOLABEL, 0, 15, 15, 1);
                auto* spd = createChild<CommandSlider>(LIT("Speed"),        CMDNAMES("wingsspeed"),  NOLABEL, 0, 15,  5, 1);
                createChild<CommandWingsSetSpeedFromMax>(mx, spd);
                tog->m_min   = mn;
                tog->m_max   = mx;
                tog->m_speed = spd;
            }
        };
    }
}
