#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandList.hpp"
#include "Commands/Widgets/CommandSliderFloat.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"
#include "Commands/Widgets/CommandToggle.hpp"

namespace Stand
{
    namespace
    {
        class CommandGravityMultSlider : public CommandSliderFloat
        {
            CommandToggle* m_not_in_aircraft = nullptr;
            bool m_ticking = false;

        public:
            explicit CommandGravityMultSlider(CommandList* parent)
                : CommandSliderFloat(parent, LIT("Gravity Multiplier"), CMDNAMES("gravitymultiplier"),
                    NOLABEL, -10000, 10000, 100, 10) {}

            void SetNotInAircraft(CommandToggle* t) { m_not_in_aircraft = t; }

            ~CommandGravityMultSlider() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                float grav = (float(value) / 100.f) * 9.8000002f;
                if (m_not_in_aircraft && m_not_in_aircraft->m_on &&
                    VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh)))
                    grav = 9.8000002f;
                FieldAt<float>(cveh, 0x0C9C) = grav;
            }

            void onChange(Click& click, int) override
            {
                if (click.isAuto()) return;
                if (value != 100)
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
                        if (!veh) return;
                        void* cveh = GetVehPtr(veh);
                        if (cveh) FieldAt<float>(cveh, 0x0C9C) = 9.8000002f;
                    });
                }
            }
        };

        class CommandListGravityMult : public CommandList
        {
        public:
            explicit CommandListGravityMult(CommandList* parent)
                : CommandList(parent, LIT("Gravity Multiplier"), CMDNAMES_0())
            {
                auto* slider = createChild<CommandGravityMultSlider>();
                auto* notAircraft = createChild<CommandToggle>(
                    LIT("Not In Aircraft"), CMDNAMES("gravitynotinaircraft"),
                    LIT("Skips the gravity multiplier for aircraft, keeping them at normal gravity."));
                slider->SetNotInAircraft(notAircraft);
            }
        };
    }
}
