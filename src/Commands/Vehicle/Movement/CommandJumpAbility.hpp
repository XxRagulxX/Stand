#pragma once
#include "Commands/Vehicle/Movement/CommandMovementShared.hpp"
#include "Commands/Widgets/CommandSlider.hpp"
#include "Commands/Widgets/CommandTickDispatch.hpp"

namespace Stand
{
    namespace
    {
        class CommandJumpAbility : public CommandSlider
        {
            bool m_ticking = false;
        public:
            explicit CommandJumpAbility(CommandList* parent)
                : CommandSlider(parent, LIT("Jump Ability"), CMDNAMES("vehjump"),
                    NOLABEL, 0, 2, 0, 1) {}

            ~CommandJumpAbility() override
            {
                if (m_ticking) CommandTickDispatch::RemoveCommand(this);
            }

            std::string getValueText() const override
            {
                switch (value)
                {
                case 1: return "Enabled";
                case 2: return "Disabled";
                default: return "Don't Override";
                }
            }

            void onTick() override
            {
                int veh = MovVehicle();
                if (!veh) return;
                void* cveh = GetVehPtr(veh);
                if (!cveh) return;
                void* model = FieldAt<void*>(cveh, 0x20);
                if (!model) return;
                if (value == 1)
                    FieldAt<uint8_t>(model, 0x57C + 15) |= static_cast<uint8_t>(1u << 5);
                else
                    FieldAt<uint8_t>(model, 0x57C + 15) &= static_cast<uint8_t>(~(1u << 5));
            }

            void onChange(Click& click, int) override
            {
                if (value == 0)
                {
                    if (m_ticking) { CommandTickDispatch::RemoveCommand(this); m_ticking = false; }
                }
                else
                {
                    if (!m_ticking) { CommandTickDispatch::AddCommand(this); m_ticking = true; }
                }
            }
        };
    }
}
