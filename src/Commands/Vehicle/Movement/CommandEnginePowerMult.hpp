#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"

namespace Stand
{
    namespace
    {
        class CommandEnginePowerMult : public CommandSliderFloat
        {
            bool m_ticking = false;
        public:
            explicit CommandEnginePowerMult(CommandList* parent)
                : CommandSliderFloat(parent, LIT("Engine Power Multiplier"), CMDNAMES("enginepowermult"),
                    NOLABEL, 100, 2000, 100, 50) {}

            ~CommandEnginePowerMult() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                VEHICLE::MODIFY_VEHICLE_TOP_SPEED(veh, getFloatValue() * 10.f);
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                float fval = getFloatValue();
                if (fval != 1.f)
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
                        if (veh) VEHICLE::MODIFY_VEHICLE_TOP_SPEED(veh, 10.f);
                    });
                }
            }
        };
    }
}
