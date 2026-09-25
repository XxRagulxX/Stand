#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"

namespace Stand
{
    namespace
    {
        class CommandSpeedLimit : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandSpeedLimit(CommandList* parent)
                : CommandSlider(parent, LIT("Speed Limit (KMH)"), CMDNAMES("speedlimit"),
                    LIT("Sets the speed limit of vehicles that you pilot or drive. (Default: 540)"),
                    0, 10000, 540, 20) {}

            ~CommandSpeedLimit() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                float maxSpd = (value == 540) ? 150.f : ((float)value / 3.6f);
                ENTITY::SET_ENTITY_MAX_SPEED(veh, maxSpd);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                if (value != 540)
                {
                    if (!m_ticking)
                    {
                        CommandTickDispatch::AddCommand(this);
                        m_ticking = true;
                    }
                }
                else
                {
                    if (m_ticking)
                    {
                        CommandTickDispatch::RemoveCommand(this);
                        m_ticking = false;
                    }
                    click.ensureScriptThread([] {
                        int veh = MovVehicle();
                        if (veh) ENTITY::SET_ENTITY_MAX_SPEED(veh, 150.f);
                    });
                }
            }
        };
    }
}
